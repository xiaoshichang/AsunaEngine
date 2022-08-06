using System;
using System.Collections;
using System.Collections.Generic;
using Asuna.Foundation;
using UnityEngine;
using Logger = Asuna.Foundation.Logger;

namespace Asuna.Application
{
    public class ApplicationRoot : MonoBehaviour
    {
        private void _InitGMManager()
        {
            var assemblyList = new List<string>()
            {
                "Asuna.GamePlay",
                "Asuna.GamePlayCore",
                "Asuna.UI"
            };
            GMManager.Init(assemblyList);
            gameObject.AddComponent<GMTerminal>();
        }

        private void _InitLogManager()
        {
            Logger.Init();
        }

        private void _InitNetwork()
        {
            NetworkMgr.Instance.Init();
        }
        
        void Awake()
        {
            _InitGMManager();
            _InitLogManager();
            _InitNetwork();
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

