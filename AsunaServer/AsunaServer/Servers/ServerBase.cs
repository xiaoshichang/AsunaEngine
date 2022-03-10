using AsunaNetwork;
using AsunaServer.Config;

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
        _InternalNetwork.Init(_ServerConfig.InternalIP, _ServerConfig.InternalPort);
    }

    public virtual void Uninit()
    {
    }

    protected virtual void ProcessNetworkEvents()
    {
    }

    protected virtual void ProcessTimerEvents()
    {
    }
    
    public virtual void Run()
    {
        _InternalNetwork.StartListen();
        while (_QuitFlag)
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