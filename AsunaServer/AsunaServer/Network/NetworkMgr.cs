
using System;
using System.Collections.Generic;
using System.Net.Sockets;

namespace AsunaServer.Network;


public class NetworkMgrTcp : NetworkMgrBase
{
    public override void Init(string? address, int port)
    {
    }

    public override void StartListen()
    {
        Console.Out.WriteLine("StartListen");
    }

    private TcpSession CreateSession(Socket socket)
    {
        TcpSession session = new TcpSession(socket)
        {
            OnEventCallback = AddEvent
        };
        return session;
    }

    private void OnAcceptConnection(NetworkEvent evt)
    {
        if (evt.AcceptSocket == null)
        {
            // warning
            return;
        }
        var session = CreateSession(evt.AcceptSocket);
        _AllSessions.Add(session.SessionID, session);
        OnAccepConnectionCallback?.Invoke(evt);
        session.StartReceiving();
    }

    private void OnDisconnect(NetworkEvent evt)
    {
        OnDisconnectCallback?.Invoke(evt);
    }

    private void OnReceiveMessage(NetworkEvent evt)
    {
        OnReceiveMessageCallback?.Invoke(evt);
    }

    private void OnSendMessage(NetworkEvent evt)
    {
    }

    public override void ProcessNetworkEvents()
    {
        lock (_EventQueue)
        {
            while (_EventQueue.Count > 0)
            {
                var evt = _EventQueue.Dequeue();
                switch (evt.EventType)
                {
                    case NetworkEventType.Accept:
                    {
                        OnAcceptConnection(evt);
                        break;
                    }
                    case NetworkEventType.Disconnect:
                    {
                        OnDisconnect(evt);
                        break;
                    }
                    case NetworkEventType.OnReceive:
                    {
                        OnReceiveMessage(evt);
                        break;
                    }
                    case NetworkEventType.OnSend:
                    {
                        OnSendMessage(evt);
                        break;
                    }
                }
            }
        }
    }

    public override void AddEvent(NetworkEvent evt)
    {
        lock (_EventQueue)
        {
            _EventQueue.Enqueue(evt);
        }
    }

    
    private readonly Queue<NetworkEvent> _EventQueue = new Queue<NetworkEvent>();
    private readonly Dictionary<uint, TcpSession> _AllSessions = new Dictionary<uint, TcpSession>();

}