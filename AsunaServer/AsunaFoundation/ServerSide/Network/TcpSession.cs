using System.Net.Sockets;

#pragma warning disable CS8602

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
        try
        {
            var receiveSize = _Socket.EndReceive(ar);
            if (receiveSize == 0)
            {
                DisconnectFromSession(DisconnectReason.CloseByRemote);
                return;
            }
        
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
        catch (Exception e)
        {
            Logger.LogError($"OnReceiveHeader Exception {e.Message}");
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
        try
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
        catch (Exception e)
        {
            Logger.LogError($"OnReceiveJsonMsg Exception {e.Message}");
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

    public void DoDisconnect()
    {
        _Socket.Shutdown(SocketShutdown.Both);
        _Socket.Close();
    }

    /// <summary>
    /// send message to client side.
    /// note that this function must be called from main thread.
    /// </summary>
    public void SendMsg(MsgBase msg)
    {
        lock (_SendQueue)
        {
            _SendQueue.Enqueue(msg);
            if (_SendQueue.Count == 1)
            {
                var msgToSend = _SendQueue.Peek();
                msgToSend.DumpToBuffer();
                DoSendMsg(msgToSend);
            }
        }
    }
    

    private void DoSendMsg(MsgBase msg)
    {
        _Socket.BeginSend(msg.Buffer, msg.BufferOffset, msg.Buffer.Length, SocketFlags.None, OnSend, msg);
    }

    private void OnSend(IAsyncResult ar)
    {
        var sent = _Socket.EndSend(ar);
        var msg = ar.AsyncState as MsgBase;
        msg.BufferOffset += sent;
        if (msg.BufferOffset == msg.Buffer.Length)
        {
            lock (_SendQueue)
            {
                _SendQueue.Dequeue();
                if (_SendQueue.Count > 0)
                {
                    var msgToSend = _SendQueue.Peek();
                    msgToSend.DumpToBuffer();
                    DoSendMsg(msgToSend);
                }
            }
        }
        else if (msg.BufferOffset < msg.Buffer.Length)
        {
            DoSendMsg(msg);
        }
        else
        {
            DisconnectFromSession(DisconnectReason.UnknownError);
        }
    }


    private readonly Socket _Socket;
    private readonly byte[] _HeaderBuffer = new byte[MsgHeader.MsgHeaderSize];
    private int _HeaderOffset = 0;
    private readonly Queue<MsgBase> _SendQueue = new();
    private readonly NetworkEventHandler _OnEventCallback;


    public uint SessionID;

    private static uint _sNextSessionID = 1;
}