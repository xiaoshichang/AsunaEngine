using System;
using System.Collections.Generic;
using System.Net.Sockets;

#pragma warning disable CS8602

namespace Asuna.Foundation
{
    public class TcpSession
    {
        public TcpSession(Socket socket, NetworkEventHandler eventHandler)
        {
            _Socket = socket;
            _OnEventCallback = eventHandler;
        }

        public void StartReceiving()
        {
            StartReceiveHeader();
        }

        private void StartReceiveHeader()
        {
            _Socket.BeginReceive(_HeaderBuffer, _HeaderOffset, PackageHeader.MsgHeaderSize, SocketFlags.None, OnReceiveHeader, null);
        }

        /// <summary>
        /// callback when receive some bytes of message header.
        /// note that this is called by other thread than main thread.
        /// </summary>
        private void OnReceiveHeader(IAsyncResult ar)
        {
            try
            {
                var receiveSize = _Socket.EndReceive(ar);
                if (receiveSize == 0)
                {
                    DisconnectFromSession(DisconnectReason.CloseByRemote);
                    return;
                }
            
                if (receiveSize == PackageHeader.MsgHeaderSize)
                {
                    PackageHeader.ParseHeader(_HeaderBuffer, out PackageHeader header);
                    _HeaderOffset = 0;
                    StartReceiveBody(header);
                }
                else if (receiveSize < PackageHeader.MsgHeaderSize)
                {
                    _HeaderOffset += receiveSize;
                    StartReceiveHeader();
                }
                else
                {
                    DisconnectFromSession(DisconnectReason.UnknownError);
                }
            }
            catch (Exception e)
            {
                Logger.LogError($"OnReceiveHeader Exception {e.Message}");
                DisconnectFromSession(DisconnectReason.UnknownError);
            }
        }

        private void StartReceiveJsonPackage(PackageJson package)
        {
            _Socket.BeginReceive(package.Buffer, package.BufferOffset, (int)package.Header.MsgSize - package.BufferOffset, SocketFlags.None, OnReceiveJsonMsg, package);
        }
        
        /// <summary>
        /// callback when receive some bytes of json body.
        /// note that this is called by other thread than main thread.
        /// </summary>
        private void OnReceiveJsonMsg(IAsyncResult ar)
        {
            try
            {
                var receiveSize = _Socket.EndReceive(ar);
                if (receiveSize == 0)
                {
                    DisconnectFromSession(DisconnectReason.CloseByRemote);
                    return;
                }
                var package = ar.AsyncState as PackageJson;
                if (package == null)
                {
                    DisconnectFromSession(DisconnectReason.UnknownError);
                    return;
                }
                package.BufferOffset += receiveSize;
                if (package.BufferOffset == package.Header.MsgSize)
                {
                    var evt = new NetworkEvent()
                    {
                        Session = this,
                        EventType = NetworkEventType.OnReceive,
                        ReceivedPackage = package
                    };
                    _OnEventCallback?.Invoke(evt);
                    StartReceiveHeader();
                }
                else if (package.BufferOffset < package.Header.MsgSize)
                {
                    StartReceiveJsonPackage(package);
                }
                else
                {
                    DisconnectFromSession(DisconnectReason.UnknownError);
                }
            }
            catch (Exception e)
            {
                Logger.LogError($"OnReceiveJsonMsg Exception {e.Message}");
                DisconnectFromSession(DisconnectReason.UnknownError);
            }
        }
        
        private void StartReceiveBody(PackageHeader header)
        {
            if (header.PackageType == PackageType.Json)
            {
                var jsonMsg = new PackageJson()
                {
                    Header = header,
                    Buffer = new byte[header.MsgSize],
                    BufferOffset = 0
                };
                StartReceiveJsonPackage(jsonMsg);
            }
            else
            {
                DisconnectFromSession(DisconnectReason.UnknownError);
            }
        }

        private void DisconnectFromSession(DisconnectReason reason)
        {
            var evt = new NetworkEvent
            {
                Session = this,
                EventType = NetworkEventType.Disconnect,
                DisconnectReason = reason
            };
            _OnEventCallback?.Invoke(evt);
        }

        public void DoDisconnect()
        {
            _Socket.Shutdown(SocketShutdown.Both);
            _Socket.Close();
        }

        /// <summary>
        /// send message to client side.
        /// note that this function must be called from main thread.
        /// </summary>
        public void SendMsg(MsgBase msg)
        {
            var package = new PackageJson()
            {
                obj = msg
            };
            
            lock (_SendQueue)
            {
                _SendQueue.Enqueue(package);
                if (_SendQueue.Count == 1)
                {
                    var msgToSend = _SendQueue.Peek();
                    msgToSend.DumpToBuffer();
                    DoSendPackage(msgToSend);
                }
            }
        }

        private void DoSendPackage(PackageBase package)
        {
            _Socket.BeginSend(package.Buffer, package.BufferOffset, package.Buffer.Length, SocketFlags.None, OnSend, package);
        }

        private void OnSend(IAsyncResult ar)
        {
            var sent = _Socket.EndSend(ar);
            var msg = ar.AsyncState as PackageBase;
            msg.BufferOffset += sent;
            if (msg.BufferOffset == msg.Buffer.Length)
            {
                lock (_SendQueue)
                {
                    _SendQueue.Dequeue();
                    if (_SendQueue.Count > 0)
                    {
                        var msgToSend = _SendQueue.Peek();
                        msgToSend.DumpToBuffer();
                        DoSendPackage(msgToSend);
                    }
                }
            }
            else if (msg.BufferOffset < msg.Buffer.Length)
            {
                DoSendPackage(msg);
            }
            else
            {
                DisconnectFromSession(DisconnectReason.UnknownError);
            }
        }

        private readonly Socket _Socket;
        private readonly byte[] _HeaderBuffer = new byte[PackageHeader.MsgHeaderSize];
        private int _HeaderOffset = 0;
        private readonly Queue<PackageBase> _SendQueue = new();
        private readonly NetworkEventHandler _OnEventCallback;
    }
}