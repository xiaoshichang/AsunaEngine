using System.Net.Sockets;
using Newtonsoft.Json;

namespace AsunaFoundation;

public class TcpSession
{
    public TcpSession(Socket socket, NetworkEventHandler eventHandler)
    {
        _Socket = socket;
        _OnEventCallback = eventHandler;
        SessionID = GenerateSessionID();
    }

    private static uint GenerateSessionID()
    {
        var id = _sNextSessionID;
        _sNextSessionID += 1;
        if (_sNextSessionID == uint.MaxValue)
        {
            _sNextSessionID = 1;
        }
        return id;
    }

    public void StartReceiving()
    {
        Logger.LogInfo("StartReceiving...");
        StartReceiveHeader();
    }

    private void StartReceiveHeader()
    {
        _Socket.BeginReceive(_HeaderBuffer, _HeaderOffset, MsgHeader.MsgHeaderSize, SocketFlags.None, OnReceiveHeader, null);
    }

    /// <summary>
    /// callback when receive some bytes of message header.
    /// note that this is called by other thread than main thread.
    /// </summary>
    private void OnReceiveHeader(IAsyncResult ar)
    {
        var receiveSize = _Socket.EndReceive(ar);
        if (receiveSize == MsgHeader.MsgHeaderSize)
        {
            MsgHeader.ParseHeader(_HeaderBuffer, out MsgHeader header);
            _HeaderOffset = 0;
            StartReceiveBody(header);
        }
        else if (receiveSize < MsgHeader.MsgHeaderSize)
        {
            _HeaderOffset += receiveSize;
            StartReceiveHeader();
        }
        else
        {
            DisconnectFromSession(DisconnectReason.UnknownError);
        }
    }

    private void StartReceiveJsonMsg(MsgJson msg)
    {
        _Socket.BeginReceive(msg.Buffer, msg.BufferOffset, (int)msg.Header.MsgSize - msg.BufferOffset, SocketFlags.None, OnReceiveJsonMsg, msg);
    }
    
    /// <summary>
    /// callback when receive some bytes of json body.
    /// note that this is called by other thread than main thread.
    /// </summary>
    private void OnReceiveJsonMsg(IAsyncResult ar)
    {
        var receiveSize = _Socket.EndReceive(ar);
        if (receiveSize == 0)
        {
            DisconnectFromSession(DisconnectReason.CloseByRemote);
            return;
        }
        var msg = ar.AsyncState as MsgJson;
        if (msg == null)
        {
            DisconnectFromSession(DisconnectReason.UnknownError);
            return;
        }
        msg.BufferOffset += receiveSize;
        if (msg.BufferOffset == msg.Header.MsgSize)
        {
            var evt = new NetworkEvent()
            {
                Session = this,
                EventType = NetworkEventType.OnReceive,
                ReceiveMsg = msg
            };
            _OnEventCallback?.Invoke(evt);
            StartReceiveHeader();
        }
        else if (msg.BufferOffset < msg.Header.MsgSize)
        {
            StartReceiveJsonMsg(msg);
        }
        else
        {
            DisconnectFromSession(DisconnectReason.UnknownError);
        }
    }
    
    private void StartReceiveBody(MsgHeader header)
    {
        if (header.MsgType == MsgType.Json)
        {
            var jsonMsg = new MsgJson
            {
                Header = header,
                Buffer = new byte[header.MsgSize],
                BufferOffset = 0
            };
            StartReceiveJsonMsg(jsonMsg);
        }
        else
        {
            DisconnectFromSession(DisconnectReason.UnknownError);
        }
    }

    private void DisconnectFromSession(DisconnectReason reason)
    {
        var evt = new NetworkEvent
        {
            Session = this,
            EventType = NetworkEventType.Disconnect,
            DisconnectReason = reason
        };
        _OnEventCallback?.Invoke(evt);
    }


    private readonly Socket _Socket;
    private readonly byte[] _HeaderBuffer = new byte[MsgHeader.MsgHeaderSize];
    private int _HeaderOffset = 0;
    
    public uint SessionID;
    public NetworkEventHandler _OnEventCallback;

    private static uint _sNextSessionID = 1;
}