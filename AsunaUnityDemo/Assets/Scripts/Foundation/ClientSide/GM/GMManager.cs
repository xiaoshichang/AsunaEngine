using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using UnityEngine;


namespace Asuna.Foundation
{
    
    public static class GMManager
    {
        private static void _RegisterGMCommand(GMAttribute attr, MethodInfo method)
        {
            if (_AllGMCommands.ContainsKey(method.Name))
            {
                Logger.Error($"duplicated gm name {method.Name}");
                return;
            }

            var cmd = new GMCommand()
            {
                Attribute = attr,
                Method = method
            };
            _AllGMCommands.Add(method.Name, cmd);
        }
        
        /// <summary>
        /// 从Assembly中搜集GM指令
        /// </summary>
        private static void _CollectGMCommandsByReflection(string assemblyName)
        {
            var assembly = Assembly.Load(assemblyName);
            foreach (var t in assembly.GetTypes())
            {
                foreach (var method in t.GetMethods())
                {
                    var attrs = method.GetCustomAttributes();
                    foreach (var attr in attrs)
                    {
                        if (attr is GMAttribute gma)
                        {
                            _RegisterGMCommand(gma, method);
                        }
                    }
                }
            }
        }

        private static object _ConvertParameter(string item, Type t)
        {
            try
            {
                if (t == typeof(int))
                {
                    return int.Parse(item);
                }

                if (t == typeof(float))
                {
                    return float.Parse(item);
                }

                if (t == typeof(string))
                {
                    return item;
                }

                if (t == typeof(uint))
                {
                    return uint.Parse(item);
                }

                if (t == typeof(double))
                {
                    return double.Parse(item);
                }
            }
            catch (Exception e)
            {
                Logger.Error($"{e.Message}! Content:{item}), Type:{t.Name}");
            }
            return null;
        }

        private static void _PrintCandidates(string prefix)
        {
            StringBuilder sb = new StringBuilder();
            sb.Append("\n");
            sb.Append(">>>>>>>>>> relative commands >>>>>>>>>>\n");
            foreach (var kv in _AllGMCommands)
            {
                if (kv.Key.StartsWith(prefix))
                {
                    var cmd = kv.Value;
                    sb.Append($"{kv.Key} - {cmd.Attribute.Desc} \n");
                }
            }
            sb.Append("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<\n");
            Logger.Info(sb.ToString());
        }
        
        /// <summary>
        /// 初始化GM系统
        /// </summary>
        /// <param name="assemblyList"> 包含GM指令的Assembly列表 </param>
        public static void Init(List<string> assemblyList)
        {
            foreach (var assemblyName in assemblyList)
            {
                _CollectGMCommandsByReflection(assemblyName);
            }
        }

        /// <summary>
        /// 执行GM命令
        /// </summary>
        public static bool Execute(string input)
        {
            var items = input.Split();
            if (items.Length <= 0)
            {
                Logger.Error("gm input invalid!");
                return false;
            }
            var cmdName = items[0];
            if (!_AllGMCommands.TryGetValue(cmdName, out var cmd))
            {
                Logger.Error($"command [{cmdName}] not found!");
                _PrintCandidates(cmdName);
                return false;
            }

            var infos = cmd.Method.GetParameters();
            var count = infos.Length;
            if (count != items.Length - 1)
            {
                Logger.Error("gm parameter count not match!");
                return false;
            }

            var parameters = new object[count];
            for (var i = 0; i < count; i++)
            {
                var info = infos[i];
                var content = items[i + 1];
                var parameter = _ConvertParameter(content, info.ParameterType);
                if (parameter == null)
                {
                    return false;
                }
                parameters[i] = parameter;
            }
            
            try
            {
                cmd.Method.Invoke(null, parameters);
                return true;
            }
            catch (Exception e)
            {
                Logger.Error(e.Message);
                return false;
            }
        }

        private static readonly Dictionary<string, GMCommand> _AllGMCommands = new();

    }
}