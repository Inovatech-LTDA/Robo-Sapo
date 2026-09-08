import socket
import subprocess
import sys
from datetime import datetime

import serial

SERVER_IP = "192.168.0.102"
SERVER_PORT = 5005


server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
print(f"UDP server listening on: {SERVER_IP}:{SERVER_PORT}...")

try:
    server_socket.bind((SERVER_IP, SERVER_PORT))
except Exception as e:
    print("Erro ao abrir o servidor: ", e)
    server_socket.close()
    sys.exit()

try:
    ser = serial.Serial("/dev/ttyUSB0", 115200, timeout=0)
    print("[+] Serial entre Arduino e Pi iniciada.")
except Exception as e:
    print("[-] Erro ao abrir a porta serial: ", e)
    sys.exit()


while True:
    try:
        data, client_address = server_socket.recvfrom(1024)
        if data:
            ser.write(data)
        timerecv = f"[+] Time of receival: {datetime.now()} | data: {data}"
        response = timerecv.encode("utf-8")
        server_socket.sendto(response, client_address)
    except Exception as e:
        print("[-] Something went wrong: ", e)
        print("[*] Closing the app, the socket and the serial...")
        server_socket.close()
        ser.close()
        break

    except KeyboardInterrupt:
        print("\n[*]Closing the app, the socket and the serial...\n")
        server_socket.close()
        ser.close()
        break
