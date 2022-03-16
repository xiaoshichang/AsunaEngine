
#pragma warning disable CS8618

namespace AsunaFoundation
{
    public enum MsgType
    {
        Json = 1,
    }
    
    public class MsgHeader
    {
        public MsgType MsgType;
        public uint MsgSize;
    }

    public class MsgJson : MsgHeader
    {
        public MsgJson()
        {
            MsgType = MsgType.Json;
        }

        public byte[] buffer;
        public object obj;
    }
}