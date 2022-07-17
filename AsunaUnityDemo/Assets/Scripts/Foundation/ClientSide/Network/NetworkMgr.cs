

using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using UnityEngine;
using Asuna.Foundation;

namespace Asuna.Foundation
{

    public enum NetState
    {
        Ready,
        Connecting,
        Connected,
        Disconnected,
    }

    public enum NetworkEventType
    {
        OnRecvMsg,
        OnDisconnectByRemote
    }

    public class NetworkEvent
    {
        public NetworkEventType EventType;
        public Exception OnConnectException;
        public PackageBase RecvPackage;
        public Exception OnDisconnectByRemoteException;
    }

    public class NetworkMgr
    {
        public static readonly NetworkMgr Instance = new NetworkMgr();

        public void Init()
        {
            _Socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp)
            {
                Blocking = true
            };

            _State = NetState.Ready;
            _ReceiveThread = new Thread(Receiving);
            _SendThread = new Thread(Sending);
        }

        public void ConnectToServer(string ip, int port, Action<Exception> callback)
        {
            if (_State != NetState.Ready)
            {
                Debug.Log("network is not ready");
                return;
            }

            IPAddress address = IPAddress.Parse(ip);
            IPEndPoint lep = new IPEndPoint(address, port);
            _State = NetState.Connecting;
            _OnConnectCallback = callback;
            
            try
            {
                _Socket.Connect(lep);
                Debug.Log($"Connected To Server {ip}:{port}");
                _State = NetState.Connected;
                callback.Invoke(null);
                _ReceiveThread.Start();
                _SendThread.Start();
            }
            catch(Exception e)
            {
                callback.Invoke(e);
            }
        }

        private void DisconnectByException(Exception exception)
        {
            NetworkEvent evt = new NetworkEvent()
            {
                EventType = NetworkEventType.OnDisconnectByRemote,
                OnDisconnectByRemoteException = exception

            };
            lock(_Events)
            {
                _Events.Enqueue(evt);
            }
        }

        private bool ReceiveHeader(out PackageHeader header)
        {
            header = null;
            var headerBuffer = new byte[PackageHeader.MsgHeaderSize];
            var bytesReceived = 0;
            while (bytesReceived != PackageHeader.MsgHeaderSize)
            {
                try
                {
                    bytesReceived += _Socket.Receive(headerBuffer, bytesReceived, PackageHeader.MsgHeaderSize - bytesReceived, SocketFlags.None);
                    if (bytesReceived == 0)
                    {
                        DisconnectByException(new EndOfStreamException());
                        return false;
                    }
                }
                catch(Exception exception)
                {
                    DisconnectByException(exception);
                    return false;
                }
            }
            PackageHeader.ParseHeader(headerBuffer, out header);
            return true;
        }

        private PackageJson ReceiveJson(int dataSize)
        {
            PackageJson package = new PackageJson()
            {
                Buffer = new byte[dataSize],
                BufferOffset = 0
            };
            
            while (package.BufferOffset != dataSize)
            {
                try
                {
                    var bytesReceived = _Socket.Receive(package.Buffer, package.BufferOffset, dataSize - package.BufferOffset, SocketFlags.None);
                    if (bytesReceived == 0)
                    {
                        DisconnectByException(new EndOfStreamException());
                        break;
                    }
                    package.BufferOffset += bytesReceived;
                }
                catch(Exception exception)
                {
                    DisconnectByException(exception);
                    break;
                }
            }
            return package;
        }

        private bool ReceiveBody(PackageHeader header, out PackageBase package)
        {
            package = null;
            if (header.PackageType == PackageType.Json)
            {
                package = ReceiveJson((int)header.MsgSize);
            }
            else
            {
                Debug.Log($"unknown msg type {header.PackageType}");
                _Socket.Close();
                return false;
            }

            return true;
        }

        private void Receiving()
        {
                
            while(true)
            {
                if (!ReceiveHeader(out PackageHeader header))
                {
                    break;
                }
                if (!ReceiveBody(header, out PackageBase msg))
                {
                    break;
                }
                NetworkEvent e = new NetworkEvent()
                {
                    EventType = NetworkEventType.OnRecvMsg,
                    RecvPackage = msg
                };
                lock(_Events)
                {
                    _Events.Enqueue(e);
                }
               
            }
        }

        public void Send(PackageBase package)
        {
            lock(_SendQueue)
            {
                _SendQueue.Enqueue(package);
            }
            _SendEvent.Set();
        }

        private void SendJson(PackageBase package)
        {
            // prepare
            var json = package as PackageJson;
            var dataBuffer = Serializer.SerializeToJson(json.obj);
            var jsonSize = dataBuffer.Length;
            package.Header.MsgSize = (uint)jsonSize;
            var headerBuffer = PackageHeader.DumpHeader(package.Header);
            var bufferSize = jsonSize + PackageHeader.MsgHeaderSize;
            
            // copy to buffer
            var buffer = new byte[bufferSize];
            headerBuffer.CopyTo(buffer, 0);
            dataBuffer.CopyTo(buffer, PackageHeader.MsgHeaderSize);
            
            // send buffer
            int bytesSent = 0;
            while (bytesSent != bufferSize)
            {
                bytesSent += _Socket.Send(buffer, bytesSent, bufferSize - bytesSent, SocketFlags.None);
            }
        }

        private void Sending()
        {
            while(true)
            {
                PackageBase package;
                lock(_SendQueue)
                {
                    package = _SendQueue.Count == 0 ? null : _SendQueue.Dequeue();
                }

                if (package == null)
                {
                    _SendEvent.Reset();
                    _SendEvent.WaitOne();
                }
                else
                {
                    if (package.Header.PackageType == PackageType.Json)
                    {
                        SendJson(package);
                    }
                    else
                    {
                        Debug.LogError("unknown msg type");
                    }
                }
            }
        }

        public void Update()
        {
            ProcessNetworkEvents();
        }

        public void Disconnect()
        {
           _Socket.Close();
        }


        private void ProcessRecvMsg(PackageBase package)
        {
            OnReceiveMsg?.Invoke(package);
        }

        private void ProcessNetworkEvents()
        {
            while(true)
            {
                NetworkEvent e;
                lock(_Events)
                {
                    if (_Events.Count == 0)
                    {
                        break;
                    }
                    e = _Events.Dequeue();
                }
                switch(e.EventType)
                {
                    case NetworkEventType.OnRecvMsg:
                    {
                        ProcessRecvMsg(e.RecvPackage);
                        break;
                    }
                    case NetworkEventType.OnDisconnectByRemote:
                    {
                        Debug.Log($"OnDisconnectByRemote {e.OnDisconnectByRemoteException.Message}");
                        break;
                    }
                    default:
                    {
                        throw new NotImplementedException(); 
                    }
                }
            }
        }

        public void OnApplicationQuit()
        {
            _ReceiveThread.Abort();
            _SendThread.Abort();
            _Socket.Close();
        }


        private Action<Exception> _OnConnectCallback; 
        private NetState _State;
        private Socket _Socket;
        private Thread _ReceiveThread;
        private Thread _SendThread;
        private readonly Queue<NetworkEvent> _Events = new Queue<NetworkEvent>();
        private readonly Queue<PackageBase> _SendQueue = new Queue<PackageBase>();
        private readonly ManualResetEvent _SendEvent = new ManualResetEvent(false);
        public Action<PackageBase> OnReceiveMsg;


    }
}