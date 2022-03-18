
using System;

#pragma warning disable CS8618

namespace AsunaFoundation
{
    public enum MsgType
    {
        Json = 1,
    }
    
    /// <summary>
    /// 
    ///     +-------------------------------------------------------------------------------------------+
    ///     +   msgType(4 bytes)     |    data_size(4 bytes)        |   data (data_size bytes)          +
    ///     +-------------------------------------------------------------------------------------------+
    /// 
    /// </summary>
    public class MsgHeader
    {
        public MsgType MsgType;
        public uint MsgSize;

        public static void ParseHeader(byte[] buffer, out MsgHeader header)
        {
            header = new MsgHeader
            {
                MsgType = (MsgType)BitConverter.ToUInt32(buffer, 0),
                MsgSize = BitConverter.ToUInt32(buffer, 4)
            };
        }

        public static byte[] DumpHeader(MsgHeader header)
        {
            var ret = new byte[MsgHeaderSize];
            var mt = BitConverter.GetBytes((uint) header.MsgType);
            var ms = BitConverter.GetBytes(header.MsgSize);
            Array.Copy(mt, 0, ret, 0, 4);
            Array.Copy(ms, 0, ret, 4, 4);
            return ret;
        }
        
        public const int MsgHeaderSize = 8;
    }

    public abstract class MsgBase
    {
        public MsgHeader Header;
        public byte[] Buffer;
        public int BufferOffset;

        public abstract void DumpToBuffer();
    }
    
    
}