using System.Text;
using Newtonsoft.Json;

#pragma warning disable CS8603

namespace Asuna.Foundation
{
    public static class Serializer
    {
        public static T DeserializeFromJson<T>(byte[] buffer)
        {
            var str = Encoding.Default.GetString(buffer);
            var obj = JsonConvert.DeserializeObject<T>(str);
            return obj;
        }

        public static byte[] SerializeToJson(object obj)
        {
            var str = JsonConvert.SerializeObject(obj);
            var buffer = Encoding.Default.GetBytes(str);
            return buffer;
        }
    }
}