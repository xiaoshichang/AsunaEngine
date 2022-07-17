using System;

#pragma warning disable CS8618
#nullable enable

namespace Asuna.Foundation
{
    /// <summary>
    /// ++++++++++++++++++++++++++++++++++++++++++++
    /// |  MsgType (4Byte)     |        body       |
    /// ++++++++++++++++++++++++++++++++++++++++++++
    /// </summary>
    public class PackageJson : PackageBase
    {
        public PackageJson()
        {
            Header = new PackageHeader(){PackageType = PackageType.Json};
        }
        public MsgBase obj;
        
        public override void DumpToBuffer()
        {
            var bodyBuffer = Serializer.SerializeToJson(obj);
            Header.MsgSize = (uint)bodyBuffer.Length + 4;
            Buffer = new byte[Header.MsgSize + PackageHeader.MsgHeaderSize];
            BufferOffset = 0;
            var headerBuffer = PackageHeader.DumpHeader(Header);
            var msgTypeBuffer = BitConverter.GetBytes((int)obj.MsgType);
            headerBuffer.CopyTo(Buffer, 0);
            msgTypeBuffer.CopyTo(Buffer, PackageHeader.MsgHeaderSize);
            bodyBuffer.CopyTo(Buffer, PackageHeader.MsgHeaderSize + 4);
        }

        public int GetMsgType()
        {
            return BitConverter.ToInt32(Buffer, 0);
        }

        public MsgBase? GetMsg(Type t)
        {
            var o = Serializer.DeserializeFromJson(Buffer, t, 4);
            var msg = o as MsgBase;
            return msg;
        }
        
    }

}