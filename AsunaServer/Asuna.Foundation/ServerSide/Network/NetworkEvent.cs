using System.Net.Sockets;

#pragma warning disable CS8618

namespace Asuna.Foundation
{
    public enum NetworkEventType
    {
        OnConnect,
        Accept,
        OnReceive,
        OnSend,
        Disconnect,
    }

    public enum DisconnectReason
    {
        UnknownError,
        CloseByRemote,
    }


    public class NetworkEvent
    {
        /// <summary>
        /// the session which this event belong
        /// </summary>
        public TcpSession Session;
    
        /// <summary>
        /// event type
        /// </summary>
        public NetworkEventType EventType;
    
        /// <summary>
        /// indicate the accepted socket when event type is Accept
        /// </summary>
        public Socket AcceptSocket;

        /// <summary>
        /// indicate the connected socket when event type is OnConnect
        /// </summary>
        public Socket ConnectedSocket;
    
        /// <summary>
        /// indicate the reason why a session is disconnected when event type is Disconnect
        /// </summary>
        public DisconnectReason DisconnectReason;
    
        /// <summary>
        /// indicate the received package when event type is OnReceive
        /// </summary>
        public PackageBase ReceivedPackage;
    }

    public delegate void NetworkEventHandler(NetworkEvent evt);
}



