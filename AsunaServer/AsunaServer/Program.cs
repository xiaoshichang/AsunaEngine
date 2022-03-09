using System;
using AsunaNetwork;

using AsunaEntity;

namespace AsunaServer
{
    public static class Program
    {
        static void Main(string[] args)
        {
            var avatar = new Avatar();
            Console.Out.WriteLine($"avatar guid: {avatar.guid}");
        }
    }
}