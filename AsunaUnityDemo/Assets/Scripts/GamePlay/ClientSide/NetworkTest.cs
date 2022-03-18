using System;
using System.Collections;
using System.Collections.Generic;
using System.Text;
using AsunaFoundation;
using UnityEngine;

namespace AsunaGamePlay
{
    public class NetworkTest : MonoBehaviour
    {
        private void Start()
        {
            NetworkMgr.Instance.Init();
            NetworkMgr.Instance.ConnectToServer("127.0.0.1", 40001, ConnectCallback);
            NetworkMgr.Instance.OnReceiveMsg = OnReceiveMsg;
        }

        private void ConnectCallback(Exception e)
        {
            if (e != null)
            {
                Debug.Log(e.Message);
                return;
            }
            Debug.Log("Connect to server ok!");
            StartCoroutine(SendHello());
        }

        private IEnumerator SendHello()
        {
            while (true)
            {
                yield return new WaitForSeconds(1);
                Debug.Log("Send ping");
                var msg = new MsgJson()
                {
                    obj = new MsgCSPing()
                    {
                        Timestamp = TimeStamp.GetTimeStampInMilliseconds()
                    }
                };
                NetworkMgr.Instance.Send(msg);
            }
        }

        private void OnReceiveMsg(MsgBase msg)
        {
            var pong = Serializer.DeserializeFromJson<MsgSCPong>(msg.Buffer);
            var diff = TimeStamp.GetTimeStampInMilliseconds() - pong.Timestamp;
            Debug.Log($"ping - pong {diff}");
        }

        private void Update()
        {
            NetworkMgr.Instance.Update();
        }

        private void OnApplicationQuit()
        {
        }
    }
}