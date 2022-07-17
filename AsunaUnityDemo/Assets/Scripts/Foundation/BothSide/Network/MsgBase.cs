
using System.Runtime.Serialization;

namespace Asuna.Foundation
{
    
    [DataContract]
    public class MsgBase
    {
        [DataMember]
        public int MsgType;
    }
    
}

