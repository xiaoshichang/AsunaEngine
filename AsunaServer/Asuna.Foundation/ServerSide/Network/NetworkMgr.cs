
using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;

#pragma warning disable CS8600
#pragma warning disable CS8604
#pragma warning disable CS8602

namespace Asuna.Foundation
{
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
        /// this is called from main thread
        /// </summary>
        public override void ConnectTo(string ip, int port)
        {
            Logger.LogInfo($"ConnectTo {ip}:{port}");
            var socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            IPEndPoint lep = new IPEndPoint(IPAddress.Parse(ip), port);
            socket.BeginConnect(lep, OnAsyncConnectTo, socket);
        }

        
        /// <summary>
        /// this is called from network thread
        /// </summary>
        private void OnAsyncConnectTo(IAsyncResult ar)
        {
            var socket = ar.AsyncState as Socket;
            socket.EndConnect(ar);
            var connectEvent = new NetworkEvent()
            {
                ConnectedSocket = socket,
                EventType = NetworkEventType.OnConnect
            };
            lock (_EventQueue)
            {
                _EventQueue.Enqueue(connectEvent);
            }
        }

        /// <summary>
        /// this is called from main thread
        /// </summary>
        private void OnConnectTo(NetworkEvent e)
        {
            var session = new TcpSession(e.ConnectedSocket, AddEvent);
            e.Session = session;
            _AllSessions.Add(session);
            OnConnectToCallback?.Invoke(e);
        }

        /// <summary>
        /// this callback is called by network thread
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
        /// this is called from main thread
        /// </summary>
        private void OnAcceptConnection(NetworkEvent evt)
        {
            Logger.LogInfo($"accept new connection {evt.AcceptSocket.RemoteEndPoint}");
            var session = new TcpSession(evt.AcceptSocket, AddEvent);
            _AllSessions.Add(session);
            OnAcceptConnectionCallback?.Invoke(evt);
            session.StartReceiving();
        }

        /// <summary>
        /// this is called from main thread
        /// </summary>
        private void OnDisconnect(NetworkEvent evt)
        {
            evt.Session.DoDisconnect();
            _AllSessions.Remove(evt.Session);
            OnDisconnectCallback?.Invoke(evt);
        }

        /// <summary>
        /// this is called from main thread
        /// </summary>
        private void OnReceivePackage(NetworkEvent evt)
        {
            OnReceivePackageCallback?.Invoke(evt);
        }

        private void OnSendPackage(NetworkEvent evt)
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
                            case NetworkEventType.OnConnect:
                            {
                                OnConnectTo(evt);
                                break;
                            }
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
                                OnReceivePackage(evt);
                                break;
                            }
                            case NetworkEventType.OnSend:
                            {
                                OnSendPackage(evt);
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
            // after adding a net event, notify main thread immediately
            LoopEvent.Set();
        }

        private IPEndPoint? _ListenEndPoint;
        private readonly Socket _ListenSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
        private readonly Queue<NetworkEvent> _EventQueue = new Queue<NetworkEvent>();
        
        /// <summary>
        /// maintains all exist sessions
        /// </summary>
        private readonly HashSet<TcpSession> _AllSessions = new HashSet<TcpSession>();
    }
}


