import serial
from sys import platform
import time
import serial.tools.list_ports
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
    serial_port.timeout = 10
    try:
        serial_port.open()
        break
    except:
        print('Serial error')
        exit(0)



print('connected')
while True:

    response = serial_port.readline().decode('UTF-8')
    print(response)
