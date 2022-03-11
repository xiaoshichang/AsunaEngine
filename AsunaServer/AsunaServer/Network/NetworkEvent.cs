namespace AsunaServer.Network;


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
}

public delegate void NetworkEventHandler(NetworkEvent evt);
