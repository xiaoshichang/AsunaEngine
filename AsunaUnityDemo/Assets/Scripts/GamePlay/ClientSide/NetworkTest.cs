using System;
using System.Collections;
using System.Collections.Generic;
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
            yield return new WaitForSeconds(1);
            Debug.Log("Send ping");
            var msg = new MsgJson();
            var data = new Dictionary<string, string>
            {
                ["request"] = "ping"
            };
            msg.obj = data;
            NetworkMgr.Instance.Send(msg);
        }

        private void Update()
        {
            NetworkMgr.Instance.Update();
        }
    }
}