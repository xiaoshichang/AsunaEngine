namespace Asuna.Foundation
{
    public abstract class NetworkMgrBase
    {
        public abstract void Init(string address, int port);
        public abstract void StartListen();
        public abstract void ProcessNetworkEvents();
        public abstract void AddEvent(NetworkEvent evt);
    
        public NetworkEventHandler? OnReceiveMessageCallback;
        public NetworkEventHandler? OnAcceptConnectionCallback;
        public NetworkEventHandler? OnDisconnectCallback;
        public ManualResetEvent LoopEvent = new ManualResetEvent(false);
    }
}

