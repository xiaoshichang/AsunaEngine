using System.Threading;

namespace Asuna.Foundation
{
    public abstract class NetworkMgrBase
    {
        public abstract void Init(string address, int port);
        public abstract void StartListen();
        public abstract void ConnectTo(string address, int port);
        public abstract void ProcessNetworkEvents();
        public abstract void AddEvent(NetworkEvent evt);
    
        public NetworkEventHandler? OnReceivePackageCallback;
        public NetworkEventHandler? OnAcceptConnectionCallback;
        public NetworkEventHandler? OnDisconnectCallback;
        public NetworkEventHandler? OnConnectToCallback;
        public ManualResetEvent LoopEvent = new ManualResetEvent(false);
    }
}

