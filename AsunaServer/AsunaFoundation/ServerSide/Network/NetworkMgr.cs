
using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;

#pragma warning disable CS8600
#pragma warning disable CS8604

namespace AsunaFoundation;


public class NetworkMgrTcp : NetworkMgrBase
{
    public override void Init(string address, int port)
    {
        try
        {
            _ListenEndPoint = new IPEndPoint(IPAddress.Parse(address), port);
        }
        catch (Exception e)
        {
            Logger.LogError($"Parse IP Address fail: {e.Message}!");
            throw;
        }
    }

    public override void StartListen()
    {
        Logger.LogInfo($"Start listening at {_ListenEndPoint}...");
        _ListenSocket.Bind(_ListenEndPoint);
        _ListenSocket.Listen(10);
        _ListenSocket.BeginAccept(OnAsyncAccept, null);
    }

    /// <summary>
    /// note that is callback is called by other thread than main thread
    /// </summary>
    private void OnAsyncAccept(IAsyncResult ar)
    {
        var handler = _ListenSocket.EndAccept(ar);
        var acceptEvent = new NetworkEvent()
        {
            AcceptSocket = handler,
            EventType = NetworkEventType.Accept
        };
        lock (_EventQueue)
        {
            _EventQueue.Enqueue(acceptEvent);
        }
        _ListenSocket.BeginAccept(OnAsyncAccept, null);
    }

    /// <summary>
    /// this callback is called by main thread
    /// </summary>
    private void OnAcceptConnection(NetworkEvent evt)
    {
        Logger.LogInfo($"accept new connection {evt.AcceptSocket.RemoteEndPoint}");
        var session = new TcpSession(evt.AcceptSocket, AddEvent);
        _AllSessions.Add(session.SessionID, session);
        OnAcceptConnectionCallback?.Invoke(evt);
        session.StartReceiving();
    }

    private void OnDisconnect(NetworkEvent evt)
    {
        evt.Session.DoDisconnect();
        _AllSessions.Remove(evt.Session.SessionID);
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
                try
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
                catch (Exception e)
                {
                    Logger.LogError($"Exception when ProcessNetworkEvents {e.StackTrace}");
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

    private IPEndPoint? _ListenEndPoint;
    private readonly Socket _ListenSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
    private readonly Queue<NetworkEvent> _EventQueue = new Queue<NetworkEvent>();
    private readonly Dictionary<uint, TcpSession> _AllSessions = new Dictionary<uint, TcpSession>();

}