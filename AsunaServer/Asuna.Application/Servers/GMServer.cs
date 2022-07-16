using Asuna.Foundation;

namespace Asuna.Application
{
    public class GMServer : ServerBase
    {
        public GMServer(ServerGroupConfig groupConfig, GMServerConfig serverConfig) : base(groupConfig, serverConfig)
        {
            TimerMgr.RegisterTimer(true,2000, TimeoutTest3);
            tid = TimerMgr.RegisterTimer(3000, TimeoutTest1);
            TimerMgr.RegisterTimer(1000, TimeoutTest2);
        }

        public void TimeoutTest1()
        {
            Logger.LogInfo("Time out1!");
        }
    
        public void TimeoutTest2()
        {
            Logger.LogInfo("Time out2!");
        }
    
        public void TimeoutTest3()
        {
            Logger.LogInfo("Time out3!");
            TimerMgr.UnregisterTimer(tid);
        }

        private ulong tid = 0;


    }
}

