using Asuna.Foundation;

#pragma warning disable CS8604
#pragma warning disable CS8602


namespace Asuna.Application;

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
        _InternalNetwork.OnAcceptConnectionCallback = OnInternalAcceptConnection;
        _InternalNetwork.OnDisconnectCallback = OnInternalDisconnect;
        _InternalNetwork.OnReceiveMessageCallback = OnInternalReceiveMessage;
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
        TimerMgr.Tick();
    }

    /// <summary>
    /// callback when a internal node accepted
    /// </summary>
    protected virtual void OnInternalAcceptConnection(NetworkEvent evt)
    {
        Logger.LogInfo($"OnInternalAcceptConnection");
    }

    /// <summary>
    /// callback when a internal node disconnected
    /// </summary>
    protected virtual void OnInternalDisconnect(NetworkEvent evt)
    {
        Logger.LogInfo($"OnInternalDisconnect {evt.Session.SessionID}");
    }
    
    /// <summary>
    /// callback when receive a network message from internal network
    /// </summary>
    protected virtual void OnInternalReceiveMessage(NetworkEvent evt)
    {
        var msg = evt.ReceiveMsg as MsgJson;
        var ping = Serializer.DeserializeFromJson<MsgCSPing>(msg.Buffer);
        var rsp = new MsgJson
        {
            obj = new MsgSCPong()
            {
                Timestamp = ping.Timestamp
            }
        };
        evt.Session.SendMsg(rsp);
    }
    
    public virtual void Run()
    {
        _InternalNetwork.StartListen();
        while (!_QuitFlag)
        {
            ProcessNetworkEvents();
            ProcessTimerEvents();
            _InternalNetwork.LoopEvent.Reset();
            _InternalNetwork.LoopEvent.WaitOne(10);
        }
    }

    protected bool _QuitFlag = false;
    protected readonly ServerGroupConfig _ServerGroupConfig;
    protected readonly ServerConfigBase _ServerConfig;
    protected readonly NetworkMgrBase _InternalNetwork = new NetworkMgrTcp();
}