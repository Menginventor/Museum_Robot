import socket
import serial
from sys import platform
import time
import serial.tools.list_ports
import sys
serial_port = serial.Serial()
ports =  serial.tools.list_ports.comports(include_links=False)

def find_nucleo_port(ports):
    if platform == 'win32':
        target_description = 'STMicroelectronics STLink Virtual COM Port'
    elif platform =='darwin':
        target_description = 'STM32 STLink'
    elif platform =='linux':
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

TCP_IP = '172.20.10.4'
TCP_PORT = 8888
BUFFER_SIZE = 1024  # Normally 1024, but we want fast response

print('program start')
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((TCP_IP, TCP_PORT))
s.listen(1)

conn, addr = s.accept()
print ('Connection address:', addr)
while True:
    print('waiting..')
    data = conn.recv(BUFFER_SIZE)
    if not data:
        continue
    print ("received data:", data)
    serial_port.write(data)
    conn.send(data)  # echo
conn.close()
