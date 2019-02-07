
import time

import pygame
import sys
import socket
import sys

HOST, PORT = "172.20.10.4", 8888


def TCP_transive(data):

        # Connect to server and send data

        sock.sendall(data)
        # Receive data from the server and shut down
        received = sock.recv(1024)
        print(received)




pygame.init()

pygame.display.set_mode((100, 100))
w_pressed = False
s_pressed = False
a_pressed = False
d_pressed = False
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
    sock.connect((HOST, PORT))
    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                sys.exit()
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_w:
                    #print('Forward')
                    
                    TCP_transive(b'$L100000 R-100000\r\n')

                elif event.key == pygame.K_s:
                    #print('Backward')

                    TCP_transive(b'$L-100000 R100000\r\n')
                if event.key == pygame.K_a:
                    #print('Left')

                    TCP_transive(b'$L-100000 R-100000\r\n')
                elif event.key == pygame.K_d:
                    print('Right')

                    TCP_transive(b'$L100000 R100000\r\n')
                    print('sending')
            if event.type == pygame.KEYUP:
                #print('key up')

                TCP_transive(b'$ L0  R0\r\n')

