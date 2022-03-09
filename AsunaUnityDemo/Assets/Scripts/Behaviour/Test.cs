using System;
using AsunaEntity;
using UnityEngine;

namespace Behaviour
{
    public class Test : MonoBehaviour
    {
        private void Start()
        {
            var avatar = new PlayerAvatar();
            Debug.Log(avatar.guid);
        }
    }
}