import socket
import sys

HOST, PORT = "172.20.10.4", 8888

data = 'demo tcp data123456'

# Create a socket (SOCK_STREAM means a TCP socket)
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
    # Connect to server and send data
    sock.connect((HOST, PORT))



    sock.sendall(b'$L50000 R50000\r\n')

    # Receive data from the server and shut down
    received = sock.recv(1024)

#print("Sent:     {}".format(data))
#print("Received: {}".format(received))

print(received)