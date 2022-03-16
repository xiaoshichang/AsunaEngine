using System.Net.Sockets;

#pragma warning disable CS8618

namespace AsunaFoundation;


public enum NetworkEventType
{
    Accept,
    OnReceive,
    OnSend,
    Disconnect,
}


public class NetworkEvent
{
    public NetworkEventType EventType;
    public Socket AcceptSocket;
}

public delegate void NetworkEventHandler(NetworkEvent evt);
