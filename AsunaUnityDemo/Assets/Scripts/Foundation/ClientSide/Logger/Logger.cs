using System;
using UnityEditor.VersionControl;
using UnityEngine;

namespace Asuna.Foundation
{
    public static class Logger
    {
        private static string _RecordFormat = "[{0}][{1}] - {2}";

        private static void _SendToConsole(LogLevel level, string record)
        {
            if (level == LogLevel.Info)
            {
                Debug.Log(record);
            }
            else if (level == LogLevel.Warning)
            {
                Debug.LogWarning(record);
            }
            else if (level == LogLevel.Error)
            {
                Debug.LogError(record);
            }
            else
            {
                throw new NotImplementedException("unknown log level");
            }
        }
        
        private static void _Log(LogLevel level, string message)
        {
            var ts =  DateTime.Now.ToString("yyyy/MM/dd HH:mm:ss");
            var record = string.Format(_RecordFormat, ts, level.ToString(), message);
            _SendToConsole(level, record);
        }
        
        public static void Init()
        {
        }

        public static void Info(string format, params object[] args)
        {
            _Log(LogLevel.Info, string.Format(format, args));
        }

        public static void Warning(string format, params object[] args)
        {
            _Log(LogLevel.Warning, string.Format(format, args));
        }

        public static void Error(string format, params object[] args)
        {
            _Log(LogLevel.Error, string.Format(format, args));
        }

    }
}