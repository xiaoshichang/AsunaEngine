using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AsunaEditor.Structures
{
    public enum LogLevel
    {
        Info,
        Warning,
        Error,
    }

    public struct LogRecord
    {
        public LogRecord(LogLevel level, string message)
        {
            Level = level;
            Message = message;
            Timestamp = DateTime.Now;
        }


        public LogLevel Level { get; set; }
        public DateTime Timestamp { get; set; }
        public string Message { get; set; }

        public string FullMessage
        {
            get { return $"[{Timestamp}] - {Message}"; }
        }

    }
}
