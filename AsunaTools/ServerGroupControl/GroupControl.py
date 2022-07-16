import argparse
import os
import subprocess
import json
import ctypes
import time

all_pid = []
all_handlers = []
serverExecutable = "Asuna.Application.exe"

if os.name == "nt":
    user32 = ctypes.windll.user32
    kernel32 = ctypes.windll.kernel32


def find_windows_callback(hwnd, extra):
    process_id = ctypes.c_int(0)
    process_id_point = ctypes.pointer(process_id)
    user32.GetWindowThreadProcessId(hwnd, process_id_point)
    if process_id.value in all_pid:
        all_handlers.append(hwnd)


def adjust_console_window():

    callback = ctypes.CFUNCTYPE(ctypes.c_bool, ctypes.c_int, ctypes.c_int)
    user32.EnumWindows(callback(find_windows_callback), 0)

    print(all_handlers)
    for i, handler in enumerate(all_handlers):
        row = int(i / 3)
        col = int(i % 3)
        r = user32.MoveWindow(handler, col * 800, row * 500, 800, 500, 1)


def run_server_process_nt(args, name):
    cmd = [args.exe]
    flag = 0
    flag |= subprocess.CREATE_NEW_CONSOLE   # create server program in new console, or it will exit when the console run python.exe exit
    env = os.environ.copy()
    env["ConfigPath"] = args.config
    env["ServerName"] = name
    p = subprocess.Popen(cmd, creationflags=flag, env=env)
    all_pid.append(p.pid)


def start_server_group(args):
    print("start_server_group")
    stop_server_group(args)
    server_group_config = json.load(open(args.config))
    if os.name == "nt":
        run_server_process_nt(args, "GMServer")
        run_server_process_nt(args, "DBServer")
        game_server_configs = server_group_config.get("GameServers", [])
        for i, config in enumerate(game_server_configs):
            run_server_process_nt(args, "GameServer%d" % (i + 1))
        gate_server_configs = server_group_config.get("GateServers", [])
        for i, config in enumerate(gate_server_configs):
            run_server_process_nt(args, "GateServer%d" % (i + 1))

        # sleep a small interval, or we can not find these windows use above api
        time.sleep(1)
        adjust_console_window()

    else:
        raise NotImplementedError()


def stop_server_group_windows(args):
    os.system("taskkill /f /im  " + serverExecutable)


def stop_server_group(args):
    if os.name == "nt":
        stop_server_group_windows(args)
    else:
        raise NotImplementedError()
    

def parser_args():
    parser = argparse.ArgumentParser()
    subparser = parser.add_subparsers(help="operations to be perform")

    subparserStart = subparser.add_parser("start", help="Start server group")
    subparserStart.add_argument("exe", help="server exe path")
    subparserStart.add_argument("config", help="config path of server group")
    subparserStart.set_defaults(func=start_server_group)

    subparserStop = subparser.add_parser("stop", help="Stop server group")
    subparserStop.set_defaults(func=stop_server_group)

    return parser.parse_args()


def main():
    args = parser_args()
    args.func(args)


if __name__ == "__main__":
    main()





