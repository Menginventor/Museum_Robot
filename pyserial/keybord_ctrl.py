import serial
from sys import platform
import time
import serial.tools.list_ports
import pygame
import sys
serial_port = serial.Serial()
ports =  serial.tools.list_ports.comports(include_links=False)

def find_nucleo_port(ports):
    if platform == 'win32':
        target_description = 'STMicroelectronics STLink Virtual COM Port'
    elif platform =='darwin':
        target_description = 'STM32 STLink'

    else:
        print('unknow platform',platform)
        print()
    port_name = [port.device for port in ports if port.description[0:len(target_description)] == target_description]

    return port_name

while True:#Conecting Loop
    nucleo_port = find_nucleo_port(ports)
    if len(nucleo_port) == 0:
        print('Port not found')
        exit(0)

    serial_port.port = nucleo_port[0]
    print(serial_port.port)
    serial_port.baudrate = 9600
    serial_port.timeout = 1
    try:
        serial_port.open()
        break
    except:
        print('Serial error')
        exit(0)
print('succeed full connected')




pygame.init()

pygame.display.set_mode((100, 100))

while True:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            sys.exit()
        if event.type == pygame.KEYDOWN:
            if event.key == pygame.K_w:
                #print('Forward')
                while serial_port.out_waiting>0:
                    pass
                serial_port.write(b'$L100000 R-100000\r\n')

            elif event.key == pygame.K_s:
                #print('Backward')
                while serial_port.out_waiting>0:
                    pass
                serial_port.write(b'$L-100000 R100000\r\n')
            if event.key == pygame.K_a:
                #print('Left')
                while serial_port.out_waiting>0:
                    pass
                serial_port.write(b'$L-100000 R-100000\r\n')
            elif event.key == pygame.K_d:
                print('Right')
                while serial_port.out_waiting>0:
                    pass
                serial_port.write(b'$L100000 R100000\r\n')
                print('sending')
        if event.type == pygame.KEYUP:
            #print('key up')
            while serial_port.out_waiting > 0:
                pass
            serial_port.write(b'$ L0  R0\r\n')
    if serial_port.in_waiting>0:
        response = serial_port.readline().decode('UTF-8')
        if len(response) > 0:
            print(response)
    time.sleep(0.5)