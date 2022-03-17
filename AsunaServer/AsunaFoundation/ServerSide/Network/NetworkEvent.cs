﻿using System.Net.Sockets;

#pragma warning disable CS8618

namespace AsunaFoundation;


public enum NetworkEventType
{
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
    /// indicate the reason why a session is disconnected when event type is Disconnect
    /// </summary>
    public DisconnectReason DisconnectReason;
    
    /// <summary>
    /// indicate the received message when event type is OnReceive
    /// </summary>
    public MsgBase ReceiveMsg;
}

public delegate void NetworkEventHandler(NetworkEvent evt);
