using System;
using System.Collections.Generic;
using Asuna.Foundation;

#pragma warning disable CS8604
#pragma warning disable CS8602


namespace Asuna.Application
{
    public delegate void MsgHandler(TcpSession session, MsgBase msg);
    
    
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
            _InternalNetwork.OnReceivePackageCallback = OnInternalReceivePackage;
            _InternalNetwork.OnConnectToCallback = OnInternalConnectTo;
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
        }

        /// <summary>
        /// callback when a internal node disconnected
        /// </summary>
        protected virtual void OnInternalDisconnect(NetworkEvent evt)
        {
            Logger.LogInfo($"OnInternalDisconnect {evt.Session}");
        }

        protected virtual void OnControlMsgHandShakeReq(TcpSession session, MsgBase msg)
        {
            var req = msg as ControlMsgHandShakeReq;
            _ServerToSession[req.ServerName] = session;
            var rsp = new ControlMsgHandShakeRsp(_ServerConfig.Name);
            session.SendMsg(rsp);
            Logger.LogInfo($"OnControlMsgHandShakeReq {req.ServerName}");

        }

        protected virtual void OnControlMsgHandShakeRsp(TcpSession session, MsgBase msg)
        {
            var rsp = msg as ControlMsgHandShakeRsp;
            _ServerToSession[rsp.ServerName] = session;
            Logger.LogInfo($"OnControlMsgHandShakeRsp {rsp.ServerName}");
        }

        private (Type, MsgHandler) GetMsgClassTypeAndHandlerByMsgType(int msgType)
        {
            if (msgType == (int) ControlMsgType.HandShakeReq)
            {
                return (typeof(ControlMsgHandShakeReq), OnControlMsgHandShakeReq);
            }
            else if (msgType == (int) ControlMsgType.HandShakeRsp)
            {
                return (typeof(ControlMsgHandShakeRsp), OnControlMsgHandShakeRsp);
            }
            else
            {
                throw new NotImplementedException();
            }
        }
        
        private 

        protected virtual void ProcessPackageJson(TcpSession session, PackageJson package)
        {
            var msgType = package.GetMsgType();
            var (classType, handler) = GetMsgClassTypeAndHandlerByMsgType(msgType);
            var msg = package.GetMsg(classType);
            if (msg == null)
            {
                Logger.LogWarning("message is null");
                return;
            }
            handler(session, msg);
        }
        
        /// <summary>
        /// callback when receive a network package from internal network
        /// </summary>
        protected virtual void OnInternalReceivePackage(NetworkEvent evt)
        {
            if (evt.ReceivedPackage.Header.PackageType == PackageType.Json)
            {
                ProcessPackageJson(evt.Session, evt.ReceivedPackage as PackageJson);
            }
            else
            {
                throw new NotImplementedException("Unsupported PackageType");
            }
        }
        
        /// <summary>
        /// callback when connect to a internal node
        /// </summary>
        protected virtual void OnInternalConnectTo(NetworkEvent evt)
        {
            Logger.LogInfo($"OnInternalConnectTo session id:{evt.Session}");
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
        protected readonly Dictionary<string, TcpSession> _ServerToSession = new();

    }
}
