using System.Collections.Generic;
using System.IO;
using System.Text.Json;
#pragma warning disable CS8618

namespace Asuna.Application
{
    public class CommonConfig
    {
        public string LogPath { get; set; }
    }
    
    public class ServerConfigBase
    {
        public string Name { get; set; }
        public string InternalIP { get; set; }
        public int InternalPort { get; set; }
    }
    
    public class GMServerConfig : ServerConfigBase
    {
    }
    
    public class DBServerConfig : ServerConfigBase
    {
    }
    
    public class GameServerConfig : ServerConfigBase
    {
    }
    
    public class GateServerConfig : ServerConfigBase
    {
    }
    
    public class ServerGroupConfig
    {
        public CommonConfig Common { get; set; }
        public GMServerConfig GMServer { get; set; }
        public DBServerConfig DBServer { get; set; }
        public List<GameServerConfig> GameServers { get; set; }
        public List<GateServerConfig> GateServers { get; set; }
    
        
        public ServerConfigBase? GetCurrentServerConfigByName(string servername)
        {
            if (GMServer?.Name == servername)
            {
                return GMServer;
            }
            if (DBServer?.Name == servername)
            {
                return DBServer;
            }
            foreach (var config in GameServers)
            {
                if (config.Name == servername)
                {
                    return config;
                }
            }
            foreach (var config in GateServers)
            {
                if (config.Name == servername)
                {
                    return config;
                }
            }
            return null;
        }
        
        public static ServerGroupConfig? LoadConfig(string configPath)
        {
            string context = File.ReadAllText(configPath);
            var groupConfig = JsonSerializer.Deserialize<ServerGroupConfig>(context);
            return groupConfig;
        }

        public int GetServerGroupNodesCount()
        {
            return 2 + GameServers.Count + GateServers.Count;
        }
    }
}


