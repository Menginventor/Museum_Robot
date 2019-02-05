import serial
import time
import serial.tools.list_ports
serial_port = serial.Serial()
ports =  serial.tools.list_ports.comports(include_links=False)

def find_nucleo_port(ports):
    target_description = 'STMicroelectronics STLink Virtual COM Port'

    port_name = [port.device for port in ports if port.description[0:len(target_description)] == target_description]
    return port_name

while True:#Conecting Loop
    nucleo_port = find_nucleo_port(ports)[0]
    if nucleo_port == None:
        print('Port not found')
        exit(0)
    serial_port.port = nucleo_port
    print(serial_port.port)
    serial_port.baudrate = 9600
    serial_port.timeout = 1
    try:
        serial_port.open()
        break
    except:
        print('Serial error')
        exit(0)



print('connected')
for i in range(10000):
    serial_port.write(b'test %d \r\n'%i)
    response = serial_port.readline()
    print(response)