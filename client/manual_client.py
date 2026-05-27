import socket
from messsagetypes import MessageType

# HOST = "127.0.0.1"   # server IP
HOST = "192.168.2.8"
PORT = 10001         # server port

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    while True:

        # Get the message type

        print("> ", end="")
        string = input()

        try:
            n = int(string)
        except:
            print("Not a number")
            continue

        # Send the number
        b = bytes([n])
        print(b)

        s.sendall(b)

        # Get server response and print

        data = s.recv(1024)
        print(data.decode())