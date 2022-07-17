
using System;

#pragma warning disable CS8618

namespace Asuna.Foundation
{
    public enum PackageType
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
    public class PackageHeader
    {
        public PackageType PackageType;
        public uint MsgSize;

        public static void ParseHeader(byte[] buffer, out PackageHeader header)
        {
            header = new PackageHeader
            {
                PackageType = (PackageType)BitConverter.ToUInt32(buffer, 0),
                MsgSize = BitConverter.ToUInt32(buffer, 4)
            };
        }

        public static byte[] DumpHeader(PackageHeader header)
        {
            var ret = new byte[MsgHeaderSize];
            var mt = BitConverter.GetBytes((uint) header.PackageType);
            var ms = BitConverter.GetBytes(header.MsgSize);
            Array.Copy(mt, 0, ret, 0, 4);
            Array.Copy(ms, 0, ret, 4, 4);
            return ret;
        }
        
        public const int MsgHeaderSize = 8;
    }

    
    /// <summary>
    /// formatted data sequence
    /// </summary>
    public abstract class PackageBase
    {
        public PackageHeader Header;
        public byte[] Buffer;
        public int BufferOffset;

        public abstract void DumpToBuffer();
    }
    
    
}