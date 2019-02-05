import socket
import sys

HOST, PORT = "localhost", 9999

data = 'demo tcp data123456'

# Create a socket (SOCK_STREAM means a TCP socket)
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
    # Connect to server and send data
    sock.connect((HOST, PORT))
    data = b''
    for i in range(256):
        data += bytes([i])


    sock.sendall(b'Hello World TCP')

    # Receive data from the server and shut down
    received = sock.recv(1024)

#print("Sent:     {}".format(data))
#print("Received: {}".format(received))

print(received)