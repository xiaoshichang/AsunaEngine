

using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using Newtonsoft.Json;
using UnityEngine;

namespace AsunaFoundation
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
        public MsgBase RecvMsg;
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

        private bool ReceiveHeader(out MsgHeader header)
        {
            header = null;
            var headerBuffer = new byte[MsgHeader.MsgHeaderSize];
            var bytesReceived = 0;
            while (bytesReceived != MsgHeader.MsgHeaderSize)
            {
                try
                {
                    bytesReceived += _Socket.Receive(headerBuffer, bytesReceived, MsgHeader.MsgHeaderSize - bytesReceived, SocketFlags.None);
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
            MsgHeader.ParseHeader(headerBuffer, out header);
            return true;
        }

        private MsgJson ReceiveJson(int dataSize)
        {
            MsgJson msg = new MsgJson()
            {
                Buffer = new byte[dataSize],
                BufferOffset = 0
            };
            
            while (msg.BufferOffset != dataSize)
            {
                try
                {
                    var bytesReceived = _Socket.Receive(msg.Buffer, msg.BufferOffset, dataSize - msg.BufferOffset, SocketFlags.None);
                    if (bytesReceived == 0)
                    {
                        DisconnectByException(new EndOfStreamException());
                        break;
                    }
                    msg.BufferOffset += bytesReceived;
                }
                catch(Exception exception)
                {
                    DisconnectByException(exception);
                    break;
                }
            }
            return msg;
        }

        private bool ReceiveBody(MsgHeader header, out MsgBase msg)
        {
            msg = null;
            if (header.MsgType == MsgType.Json)
            {
                msg = ReceiveJson((int)header.MsgSize);
            }
            else
            {
                Debug.Log($"unknown msg type {header.MsgType}");
                _Socket.Close();
                return false;
            }

            return true;
        }

        private void Receiving()
        {
                
            while(true)
            {
                if (!ReceiveHeader(out MsgHeader header))
                {
                    break;
                }
                if (!ReceiveBody(header, out MsgBase msg))
                {
                    break;
                }
                NetworkEvent e = new NetworkEvent()
                {
                    EventType = NetworkEventType.OnRecvMsg,
                    RecvMsg = msg
                };
                lock(_Events)
                {
                    _Events.Enqueue(e);
                }
               
            }
        }

        public void Send(MsgBase msg)
        {
            lock(_SendQueue)
            {
                _SendQueue.Enqueue(msg);
            }
            _SendEvent.Set();
        }

        private void SendJson(MsgBase msg)
        {
            // prepare
            var json = msg as MsgJson;
            var str = JsonConvert.SerializeObject(json.obj);
            var dataBuffer = Encoding.UTF8.GetBytes(str);
            var jsonSize = dataBuffer.Length;
            msg.Header.MsgSize = (uint)jsonSize;
            var headerBuffer = MsgHeader.DumpHeader(msg.Header);
            var bufferSize = jsonSize + MsgHeader.MsgHeaderSize;
            
            // copy to buffer
            var buffer = new byte[bufferSize];
            headerBuffer.CopyTo(buffer, 0);
            dataBuffer.CopyTo(buffer, MsgHeader.MsgHeaderSize);
            
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
                if (_SendQueue.Count == 0)
                {
                    _SendEvent.WaitOne();
                    continue;
                }
                MsgBase msg;
                lock(_SendQueue)
                {
                    msg = _SendQueue.Dequeue();
                }
                
                if (msg.Header.MsgType == MsgType.Json)
                {
                    SendJson(msg);
                }
                else
                {
                    Debug.LogError("unknown msg type");
                }
                _SendEvent.Reset();
            }
        }

        public void Update()
        {
            ProcessNetworkEvents();
            _SendEvent.Set();
        }

        public void Disconnect()
        {
           _Socket.Close();
        }


        private void ProcessRecvMsg(MsgBase msg)
        {
            Debug.Log($"ProcessRecvMsg {msg}");
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
                        ProcessRecvMsg(e.RecvMsg);
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
        private readonly Queue<MsgBase> _SendQueue = new Queue<MsgBase>();
        private readonly ManualResetEvent _SendEvent = new ManualResetEvent(false);

    }
}