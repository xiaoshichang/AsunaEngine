
using System;
using System.Collections.Generic;

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

    private void OnAcceptConnection(NetworkEvent evt)
    {
    }

    private void OnDisconnect(NetworkEvent evt)
    {
    }

    private void OnReceiveMessage(NetworkEvent evt)
    {
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

    protected readonly Queue<NetworkEvent> _EventQueue = new Queue<NetworkEvent>();


}