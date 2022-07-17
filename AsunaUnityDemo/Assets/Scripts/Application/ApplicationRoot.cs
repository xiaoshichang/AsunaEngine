using System;
using System.Collections;
using System.Collections.Generic;
using Asuna.Foundation;
using UnityEngine;

namespace Asuna.Application
{
    public class ApplicationRoot : MonoBehaviour
    {
        void Awake()
        {
            NetworkMgr.Instance.Init();
        }
        
        void Start()
        {
            NetworkMgr.Instance.ConnectToServer("127.0.0.1", 40001, ConnectCallback);
            NetworkMgr.Instance.OnReceiveMsg = OnReceivePackage;
        }

        void Update()
        {
            NetworkMgr.Instance.Update();
            TimerMgr.Tick();
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

        private void OnReceivePackage(PackageBase package)
        {
        }
    }
}

