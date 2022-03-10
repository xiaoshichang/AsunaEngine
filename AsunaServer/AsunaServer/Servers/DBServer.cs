using AsunaServer.Config;

namespace AsunaServer.Servers;

public class DBServer : ServerBase
{
    public DBServer(ServerGroupConfig groupConfig, DBServerConfig serverConfig) : base(groupConfig, serverConfig)
    {
    }


}