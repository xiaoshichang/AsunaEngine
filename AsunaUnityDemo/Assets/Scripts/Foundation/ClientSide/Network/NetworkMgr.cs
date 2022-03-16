

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
        public MsgHeader RecvMsg;
        public Exception OnDisconnectByRemoteException;
    }

    public class NetworkMgr
    {
        public static NetworkMgr Instance = new NetworkMgr();

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

        private bool ReceiveHeader(out int dataSize, out MsgType msgType)
        {
            byte[] header = new byte[8];
            int bytesReceived = 0;
            dataSize = 0;
            msgType = MsgType.Json;
            
            while (bytesReceived != 8)
            {
                try
                {
                    bytesReceived += _Socket.Receive(header, bytesReceived, 8 - bytesReceived, SocketFlags.None);
                    if (bytesReceived == 0)
                    {
                        DisconnectByException(new EndOfStreamException());
                        return false;
                    }
                }
                catch(Exception excetion)
                {
                    DisconnectByException(excetion);
                    return false;
                }
            }
            
            dataSize = BitConverter.ToInt32(header, 0);
            msgType = (MsgType)BitConverter.ToInt32(header, 4);
            return true;
        }

        private MsgJson ReceiveJson(int dataSize)
        {
            MsgJson msg = new MsgJson() {buffer = new byte[dataSize]};
            int bytesReceived = 0;
            
            while (bytesReceived != dataSize)
            {
                try
                {
                    bytesReceived += _Socket.Receive(msg.buffer, bytesReceived, dataSize - bytesReceived, SocketFlags.None);
                    if (bytesReceived == 0)
                    {
                        DisconnectByException(new EndOfStreamException());
                        break;
                    }
                }
                catch(Exception excetion)
                {
                    DisconnectByException(excetion);
                    break;
                }
            }
            return msg;
        }

        private bool ReceiveBody(int dataSize, MsgType msgType, out MsgHeader msg)
        {
            msg = null;
            if (msgType == MsgType.Json)
            {
                msg = ReceiveJson(dataSize);
            }
            else
            {
                Debug.Log("unknown msg type");
                _Socket.Close();
                return false;
            }

            return true;
        }

        private void Receiving()
        {
                
            while(true)
            {
                if (!ReceiveHeader(out int dataSize, out MsgType msgType))
                {
                    break;
                }
                if (!ReceiveBody(dataSize, msgType, out MsgHeader msg))
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

        public void Send(MsgHeader msg)
        {
            lock(_SendQueue)
            {
                _SendQueue.Enqueue(msg);
            }
            _SendEvent.Set();
        }

        private void SendJson(MsgHeader msg)
        {
            // prepare
            var json = msg as MsgJson;
            string str = JsonConvert.SerializeObject(json.obj);
            var data = Encoding.UTF8.GetBytes(str);
            var jsonSize = data.Length;
            var msgSize = jsonSize + 8;
            // copy to buffer
            var buffer = new byte[msgSize];
            BitConverter.GetBytes(jsonSize).CopyTo(buffer, 0);
            BitConverter.GetBytes((int)msg.MsgType).CopyTo(buffer, 4);
            data.CopyTo(buffer, 8);
            // send buffer
            int bytesSent = 0;
            while (bytesSent != msgSize)
            {
                bytesSent += _Socket.Send(buffer, bytesSent, msgSize - bytesSent, SocketFlags.None);
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
                MsgHeader msg;
                lock(_SendQueue)
                {
                    msg = _SendQueue.Dequeue();
                }
                
                if (msg.MsgType == MsgType.Json)
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


        private void ProcessRecvMsg(MsgHeader msg)
        {
            Debug.Log($"ProcessRecvMsg {msg}");
        }

        private void ProcessNetworkEvents()
        {
            while(true)
            {
                if (_Events.Count == 0)
                {
                    break;
                }
                NetworkEvent e;
                lock(_Events)
                {
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
        private Queue<NetworkEvent> _Events = new Queue<NetworkEvent>();
        private Queue<MsgHeader> _SendQueue = new Queue<MsgHeader>();
        private ManualResetEvent _SendEvent = new ManualResetEvent(false);

    }
}