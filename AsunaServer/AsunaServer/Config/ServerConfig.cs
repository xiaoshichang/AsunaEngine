using System.Collections.Generic;

namespace AsunaServer.Config;


public class CommonConfig
{
}

public class ServerConfigBase
{
    public string? Name;
    public string? InternalIP;
    public int InternalPort;
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
    public CommonConfig? Common;
    public GMServerConfig? GMServerConfig;
    public DBServerConfig? DBServerConfig;
    public List<GameServerConfig>? GameServerConfigs;
    public List<GateServerConfig>? GateServerConfigs;

    
    public ServerConfigBase? GetCurrentServerConfigByName(string servername)
    {
        if (GMServerConfig?.Name == servername)
        {
            return GMServerConfig;
        }
        if (DBServerConfig?.Name == servername)
        {
            return DBServerConfig;
        }
        if (GameServerConfigs != null)
        {
            foreach (var config in GameServerConfigs)
            {
                if (config.Name == servername)
                {
                    return config;
                }
            }
        }
        if (GateServerConfigs != null)
        {
            foreach (var config in GateServerConfigs)
            {
                if (config.Name == servername)
                {
                    return config;
                }
            }
        }
        return null;
    }
    
    
    public static ServerGroupConfig? LoadConfig(string configPath)
    {
        return null;
    }
    
    
    
}