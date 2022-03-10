namespace AsunaNetwork;


public enum NetworkEventType
{
    Accept,
    Read,
    Write,
    Disconnect,
}

public class NetworkEvent
{
    public NetworkEventType EventType;
}

public abstract class NetworkMgrBase
{
    public abstract void Init(string? address, int port);
    public abstract void StartListen();
    public abstract void Disconnect();
    public abstract void ProcessNetworkEvent(NetworkEvent evt);
}

public class NetworkMgrTcp : NetworkMgrBase
{
    public override void Init(string? address, int port)
    {
        throw new NotImplementedException();
    }

    public override void StartListen()
    {
        throw new NotImplementedException();
    }

    public override void Disconnect()
    {
        throw new NotImplementedException();
    }

    public override void ProcessNetworkEvent(NetworkEvent evt)
    {
        throw new NotImplementedException();
    }
    
    
    
    
}