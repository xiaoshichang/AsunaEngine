using System;
using Asuna.Foundation;

namespace Asuna.Application
{
    public static class Program
    {
        private static void InitLogger(ServerGroupConfig groupConfig, string serverName)
        {
            var config = new LogConfig
            {
                ConsoleLogEnable = true,
                FileLogEnable = true,
                FileLogDir = groupConfig.Common.LogPath,
                FileLogName = serverName
            };
            Logger.InitLogger(config);
        }
        
        private static ServerBase CreateServerByServerConfig(ServerGroupConfig groupConfig, ServerConfigBase serverConfig)
        {
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
            return server;
        }

        static void Main(string[] args)
        {
            // load and setup server group config
            var configPath = Environment.GetEnvironmentVariable("ConfigPath");
            if (configPath == null)
            {
                Console.Out.WriteLine($"ConfigPath not found in EnvironmentVariable!");
                return;
            }
            var groupConfig = ServerGroupConfig.LoadConfig(configPath);
            if (groupConfig == null)
            {
                Console.Out.WriteLine($"Load group config fail!");
                return;
            }
            var serverName = Environment.GetEnvironmentVariable("ServerName");
            if (serverName == null)
            {
                Console.Out.WriteLine($"ServerName not found in EnvironmentVariable!");
                return;
            }
            var serverConfig = groupConfig.GetCurrentServerConfigByName(serverName);
            if (serverConfig == null)
            {
                Console.Out.WriteLine($"{serverName} config not found in group config!");
                return;
            }
            
            InitLogger(groupConfig, serverName);
            ServerBase server = CreateServerByServerConfig(groupConfig, serverConfig);
            server.Init();
            server.Run();
            server.Uninit();
        }
    }
}