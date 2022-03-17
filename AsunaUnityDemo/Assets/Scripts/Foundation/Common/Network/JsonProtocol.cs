
#pragma warning disable CS8618

namespace AsunaFoundation
{

    public class MsgJson : MsgBase
    {
        public MsgJson()
        {
            Header = new MsgHeader(){MsgType = MsgType.Json};
        }
        public object obj;
    }

}