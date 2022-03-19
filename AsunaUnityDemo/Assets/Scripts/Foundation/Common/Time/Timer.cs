using System.Collections.Generic;
using System.Runtime.CompilerServices;

namespace AsunaFoundation
{
    public delegate void TimeoutCallback();
    
    public class ATimer
    {
        public ATimer(bool repeat, ulong delayInMilliSeconds, TimeoutCallback callback)
        {
            TimerID = NextTimerID();
            Repeat = repeat;
            Delay = delayInMilliSeconds;
            NextTimeout = Delay + TimeStamp.GetTimeStampInMilliseconds();
            TobeDelete = false;
            _Callback = callback;
        }

        public bool IsTimeout(ulong timestamp)
        {
            return timestamp >= NextTimeout;
        }

        public void OnTimeout()
        {
            if (TobeDelete)
            {
                return;
            }
            _Callback.Invoke();
        }

        public ulong NextTimerID()
        {
            GlobalTimerID += 1;
            if (GlobalTimerID == ulong.MaxValue)
            {
                GlobalTimerID = 1;
            }
            return GlobalTimerID;
        }

        public readonly ulong TimerID;
        public bool Repeat { get; }
        public ulong Delay { get; }
        public ulong NextTimeout { get; set; }
        
        /// <summary>
        /// a flag to indicate that this timer will be delete next tick
        /// </summary>
        public bool TobeDelete { get; set; }
        
        /// <summary>
        /// callback when timeout
        /// </summary>
        private readonly TimeoutCallback _Callback;

        public static ulong GlobalTimerID = 0;
    }


    public class ATimerCompare : IComparer<ATimer>
    {
        public int Compare(ATimer? x, ATimer? y)
        {
            ulong xv = x?.NextTimeout ?? 0;
            ulong yv = y?.NextTimeout ?? 0;
            return xv.CompareTo(yv);
        }
    }
    
    
    public static class TimerMgr
    {
        public static ulong RegisterTimer(ulong delayInMilliSeconds, TimeoutCallback callback)
        {
            return RegisterTimer(false, delayInMilliSeconds, callback);
        }
        
        public static ulong RegisterTimer(bool repeat, ulong delayInMilliSeconds, TimeoutCallback callback)
        {
            var timer = new ATimer(repeat, delayInMilliSeconds, callback);
            _Timers.Add(timer);
            _TidToTimers.Add(timer.TimerID, timer);
            return timer.TimerID;
        }

        public static void UnregisterTimer(ulong timerID)
        {
            if (_TidToTimers.TryGetValue(timerID, out var timer))
            {
                timer.TobeDelete = true;
            }
        }

        private static void CheckTimeoutTimers()
        {
            var timestamp = TimeStamp.GetTimeStampInMilliseconds();
            foreach (var timer in _Timers)
            {
                if (!timer.IsTimeout(timestamp))
                {
                    break;
                }
                timer.OnTimeout();
                if (timer.Repeat)
                {
                    _ToAdd.Add(timer);
                }
                else
                {
                    _ToRemove.Add(timer);
                }
            }
        }

        private static void ReBuildTimers()
        {
            foreach (var timer in _ToRemove)
            {
                _Timers.Remove(timer);
            }
            foreach (var timer in _ToAdd)
            {
                _Timers.Remove(timer);
                timer.NextTimeout += timer.Delay;
                _Timers.Add(timer);
            }
            _ToAdd.Clear();
            _ToRemove.Clear();
        }
        
        public static void Tick()
        {
            CheckTimeoutTimers();
            ReBuildTimers();
        }
        

        private static readonly List<ATimer> _ToAdd = new List<ATimer>();
        private static readonly List<ATimer> _ToRemove = new List<ATimer>();
        private static readonly SortedSet<ATimer> _Timers = new SortedSet<ATimer>(new ATimerCompare());
        private static readonly Dictionary<ulong, ATimer> _TidToTimers = new Dictionary<ulong, ATimer>();

    }
}