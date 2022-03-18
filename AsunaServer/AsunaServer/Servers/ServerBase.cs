using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using AsunaFoundation;
using AsunaServer.Config;
using Newtonsoft.Json;

#pragma warning disable CS8604
#pragma warning disable CS8602

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
        var str = Encoding.Default.GetString(msg.Buffer);
        var body = JsonConvert.DeserializeObject<Dictionary<string, string>>(str);
        Logger.LogInfo($"OnInternalReceiveMessage - {str}");
        body["message"] = "pong";
        var rsp = new MsgJson
        {
            obj = body
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
        }
    }

    protected bool _QuitFlag = false;
    protected readonly ServerGroupConfig _ServerGroupConfig;
    protected readonly ServerConfigBase _ServerConfig;
    protected readonly NetworkMgrBase _InternalNetwork = new NetworkMgrTcp();
}