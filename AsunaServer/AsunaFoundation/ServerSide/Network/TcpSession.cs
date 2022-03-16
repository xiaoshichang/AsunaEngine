using System.Net.Sockets;

namespace AsunaFoundation;

public class TcpSession
{
    public TcpSession(Socket socket)
    {
        _Socket = socket;
        SessionID = GenerateSessionID();
    }

    private uint GenerateSessionID()
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
        
    }

    private readonly Socket _Socket;

    public uint SessionID;
    public NetworkEventHandler? OnEventCallback;

    private static uint _sNextSessionID = 1;
}