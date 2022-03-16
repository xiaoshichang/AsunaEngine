using AsunaFoundation;
using AsunaServer.Config;

#pragma warning disable CS8604

namespace AsunaServer.Servers;

public abstract class ServerBase
{
    protected ServerBase(ServerGroupConfig groupConfig, ServerConfigBase serverConfig)
    {
        _ServerGroupConfig = groupConfig;
        _ServerConfig = serverConfig;
    }
    
    public virtual void Init()
    {
        Logger.LogInfo($"Server {_ServerConfig.Name} Init!");
        _InternalNetwork.Init(_ServerConfig.InternalIP, _ServerConfig.InternalPort);
        _InternalNetwork.OnAccepConnectionCallback = OnAcceptConnection;
        _InternalNetwork.OnDisconnectCallback = OnDisconnect;
        _InternalNetwork.OnReceiveMessageCallback = OnReceiveMessage;
    }

    public virtual void Uninit()
    {
    }

    protected virtual void ProcessNetworkEvents()
    {
        _InternalNetwork.ProcessNetworkEvents();
    }

    protected virtual void ProcessTimerEvents()
    {
    }

    protected virtual void OnAcceptConnection(NetworkEvent evt)
    {
    }

    protected virtual void OnDisconnect(NetworkEvent evt)
    {
    }
    
    protected virtual void OnReceiveMessage(NetworkEvent evt)
    {
    }
    
    public virtual void Run()
    {
        _InternalNetwork.StartListen();
        while (!_QuitFlag)
        {
            ProcessNetworkEvents();
            ProcessTimerEvents();
        }
    }

    protected bool _QuitFlag = false;
    protected readonly ServerGroupConfig _ServerGroupConfig;
    protected readonly ServerConfigBase _ServerConfig;
    
    protected readonly NetworkMgrBase _InternalNetwork = new NetworkMgrTcp();
}