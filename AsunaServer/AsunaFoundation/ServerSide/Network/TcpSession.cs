using System.Net.Sockets;

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
        StartReceiveHeader();
    }

    private void StartReceiveHeader()
    {
        _Socket.BeginReceive(_HeaderBuffer, _HeaderOffset, MsgHeader.MsgHeaderSize, SocketFlags.None, OnReceiveHeader, null);
    }

    private void OnReceiveHeader(IAsyncResult ar)
    {
        var receiveSize = _Socket.EndReceive(ar);
        if (receiveSize == MsgHeader.MsgHeaderSize)
        {
            
        }
        else
        {
            _HeaderOffset += receiveSize;
            StartReceiveHeader();
        }
        
    }

    private void StartReceiveBody()
    {
        
    }


    private readonly Socket _Socket;
    private readonly byte[] _HeaderBuffer = new byte[MsgHeader.MsgHeaderSize];
    private int _HeaderOffset = 0;
    
    public uint SessionID;
    public NetworkEventHandler _OnEventCallback;

    private static uint _sNextSessionID = 1;
}