import socket
import sys
from datetime import datetime

from pynput.keyboard import Key as KeyClass
from pynput.keyboard import Listener

# Remove os avisos chatos do Ruff sobre o datetime.now()
# ruff: noqa: DTZ005

SERVER_IP = "192.168.0.102"
SERVER_PORT = 5005


try:
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    client_socket.settimeout(0.5)
    client_socket.connect((SERVER_IP, SERVER_PORT))
except OSError as e:
    print(f"Erro ao abrir socket: {e}")


keys_pressed = set()


def send_keys():
    try:
        key_str = "".join(sorted(keys_pressed)) if keys_pressed else "stop"
        print(key_str)
        time_str = datetime.now()
        client_socket.send(key_str.encode("utf-8"))
        data, _addr = client_socket.recvfrom(1024)
        print(f"Resposta do servidor: {data} - {time_str}")
    except (TimeoutError, OSError) as e:
        if e == "timed out":
            print("Aviso: Tempo limite de resposta do servidor excedido (Timeout)")
        else:
            print(f"Erro: {e}")
            sys.exit(1)


def on_press(key):

    try:
        if hasattr(key, "char") and key.char is not None:
            global key_str
            match key.char:
                case "w" | "a" | "s" | "d":
                    key_str = key.char
                    keys_pressed.add(key.char)
                    send_keys()
                case _:
                    return 0

    except Exception as e:  # noqa
        print(f"Ocorreu um erro: {e}")
        sys.exit(1)



def on_release(key):
    if hasattr(key, "char") and key.char is not None and key.char in ("w", "a", "s", "d"):
            keys_pressed.discard(key.char)
            send_keys()
    if key == KeyClass.esc:
        print("Fechando o controlador.")
        sys.exit(1)


with Listener(on_press=on_press, on_release=on_release) as listener:
    try:
        listener.join()
    except Exception as e:  # noqa
        print(f"Erro: {e}")
