using System;
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
        }
    }
}