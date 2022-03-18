
using System.Text;
using Newtonsoft.Json;

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
        
        public override void DumpToBuffer()
        {
            var json = JsonConvert.SerializeObject(obj);
            var bodyBuffer = Encoding.Default.GetBytes(json);
            Header.MsgSize = (uint)bodyBuffer.Length;
            Buffer = new byte[Header.MsgSize + MsgHeader.MsgHeaderSize];
            BufferOffset = 0;
            var headerBuffer = MsgHeader.DumpHeader(Header);
            headerBuffer.CopyTo(Buffer, 0);
            bodyBuffer.CopyTo(Buffer, MsgHeader.MsgHeaderSize);
        }
    }

}