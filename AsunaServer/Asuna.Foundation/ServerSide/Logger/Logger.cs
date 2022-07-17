

using System;
using System.Collections.Generic;
using System.IO;

#pragma warning disable CS8600
#pragma warning disable CS8602
#pragma warning disable CS8618

namespace Asuna.Foundation
{
    public enum LogLevel
    {
        Info,
        Warning,
        Error
    }

    public class LogConfig
    {
        public bool ConsoleLogEnable;
        public bool FileLogEnable;
        public string? FileLogDir;
        public string? FileLogName;
    }


    public class Logger
    {

        public static void InitLogger(LogConfig config)
        {
            _config = config;
            if (_config.FileLogEnable)
            {
                if (string.IsNullOrEmpty(_config.FileLogDir) || string.IsNullOrEmpty(_config.FileLogName))
                {
                    Console.Out.WriteLine("Init Logger fail!");
                    throw new Exception("Init Logger Fail");
                }
                
                if (!Directory.Exists(_config.FileLogDir))
                {
                    Directory.CreateDirectory(_config.FileLogDir);
                }

                string logFilePath = Path.Combine(_config.FileLogDir, _config.FileLogName) + ".log";
                _LogStream = new StreamWriter(logFilePath, true);
            }
        }

        public static void LogInfo(string message)
        {
            Log(LogLevel.Info, message);
        }

        public static void LogWarning(string message)
        {
            Log(LogLevel.Warning, message);
        }

        public static void LogError(string message)
        {
            Log(LogLevel.Error, message);
        }

        private static string LevelFormat(LogLevel lv)
        {
            if (_LevelFormat.TryGetValue(lv, out string fmt))
            {
                return fmt;
            }
            return "";
        }

        private static void Log(LogLevel lv, string message)
        {
            var date = DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss");
            var lvf = LevelFormat(lv);
            var record = $"[{date}][{lvf}] - {message}";
            if (_config.ConsoleLogEnable)
            {
                Console.Out.WriteLine(record);
            }
            if (_config.FileLogEnable)
            {
                _LogStream.WriteLine(record);
                _LogStream.Flush();
            }
        }

        private static LogConfig _config;
        private static StreamWriter? _LogStream;
        private static readonly Dictionary<LogLevel, string> _LevelFormat = new()
        {
            {LogLevel.Info, "Info"},
            {LogLevel.Warning, "Warning"},
            {LogLevel.Error, "Error"}
        };
    }
}