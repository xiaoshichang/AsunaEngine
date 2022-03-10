using System;
using System.Xml.Schema;
using AsunaNetwork;

using AsunaEntity;
using AsunaServer.Config;
using AsunaServer.Servers;

namespace AsunaServer
{
    public static class Program
    {
        static void Main(string[] args)
        {
            // load and setup server group config
            var groupConfig = ServerGroupConfig.LoadConfig("");
            if (groupConfig == null)
            {
                return;
            }
            var serverConfig = groupConfig.GetCurrentServerConfigByName("");
            if (serverConfig == null)
            {
                return;
            }
            
            // create server and run it
            ServerBase server;
            if (serverConfig is GMServerConfig gmServerConfig)
            {
                server = new GMServer(groupConfig, gmServerConfig);
            }
            else if (serverConfig is DBServerConfig dbServerConfig)
            {
                server = new DBServer(groupConfig, dbServerConfig);
            }
            else if (serverConfig is GameServerConfig gameServerConfig)
            {
                server = new GameServer(groupConfig, gameServerConfig);
            }
            else if (serverConfig is GateServerConfig gateServerConfig)
            {
                server = new GateServer(groupConfig, gateServerConfig);
            }
            else
            {
                throw new Exception("unknown server type");
            }
            server.Init();
            server.Run();

            // server exit and clean up work
            {
                server.Uninit();
            }
        }
    }
}