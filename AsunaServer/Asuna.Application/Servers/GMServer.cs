using System.Collections.Generic;
using Asuna.Foundation;
using Newtonsoft.Json.Serialization;

namespace Asuna.Application
{
    public class GMServer : ServerBase
    {
        public GMServer(ServerGroupConfig groupConfig, GMServerConfig serverConfig) : base(groupConfig, serverConfig)
        {
            TimerMgr.RegisterTimer(2000, _TryConnectAllNodes);
        }
        
        private void _TryConnectAllNodes()
        {
            _InternalNetwork.ConnectTo(_ServerGroupConfig.DBServer.InternalIP, _ServerGroupConfig.DBServer.InternalPort);
            foreach (var config in _ServerGroupConfig.GameServers)
            {
                _InternalNetwork.ConnectTo(config.InternalIP, config.InternalPort);
            }
            foreach (var config in _ServerGroupConfig.GateServers)
            {
                _InternalNetwork.ConnectTo(config.InternalIP, config.InternalPort);
            }
        }
        
        protected override void OnInternalConnectTo(NetworkEvent evt)
        {
            var msg = new ControlMsgHandShakeReq(_ServerGroupConfig.GMServer.Name);
            evt.Session.StartReceiving();
            evt.Session.SendMsg(msg);
        }
        
        protected override void OnControlMsgHandShakeRsp(TcpSession session, MsgBase msg)
        {
            base.OnControlMsgHandShakeRsp(session, msg);
            if (_ServerToSession.Count >= _ServerGroupConfig.GetServerGroupNodesCount() - 1)
            {
                Logger.LogInfo("all nodes connected!");
                
            }
        }
        

    }
}

