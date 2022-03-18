using System.Runtime.Serialization;

namespace AsunaFoundation
{
    [DataContract]
    public class MsgCSPing
    {
        [DataMember]
        public ulong Timestamp { get; set; }
    }

    [DataContract]
    public class MsgSCPong
    {
        [DataMember]
        public ulong Timestamp { get; set; }
    }
}