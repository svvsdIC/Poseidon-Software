#! /usr/bin/python3
'''
Serial-Full-Duplex:  useful for talking to an Arduino running OpenRover code.

Open a comm port for full duplex operations, using a concurrent threads for
reading and writing
Print out any data read from the port
Accept input from the console, and forward it to the port

Conditionally prepend the data sent to the port with a CRC8 cyclic redundance check
'''

import serial
import threading
from time import sleep

# some global variables for inter thread communication
serial_port = serial.Serial()
stop_threads = False
use_crc = False


def prependCRC8(data):
    ''' caclulate and return the CRC8 for the openROV, sames as used by nodeJS
    crc library.  Copied the code from CCommand.cpp in the openROV software
    You must pass the full commnd including the ';' termination character
    The string passed in must be bytes, ie. the input must be encoded.
    '''

    crc = 0
    for i in data:
        extract = i
        sum = (crc ^ extract ) & 1
        for j in range(8, 0,-1):
            sum = (crc ^ extract) & 1
            crc >>= 1
            if sum:
                crc ^= 0x8C
            extract >>= 1
    return crc.to_bytes(1,"big") + data


def write():
    '''
    This is the target routine for inputing a data command (including ';'
    termination character, and converting it to bytes data to be sent out
    over the serial port.  It optionally prepends a 1 character CRC8 checksum.
    '''

    global stop_threads
    global use_crc

    print('starting write thread...')
    # repeatedly get console input and send while True: 
    while True:
        if stop_threads: 
            print('write thread is stopping...')
            break 
        try: 
            message = input().encode()
            if len(message) == 0: continue
            if use_crc: 
                message = prependCRC8(message)
            serial_port.write(message)

        except Exception as e:
            print('write thread exception')
            print({e})
            print('exception--write thread is stopping')
            stop_threads = True
            break
        sleep(.5)
    print('exiting write thread')
    
    
def read():
    '''
    read serial port and print data received.
    '''

    print('Starting read thread...')
    while True:
        if stop_threads:
            print('read thread is stopping...')
            break

        data = serial_port.read(9999).decode().lstrip()
        data.strip('\n')
        if len(data) > 0:
            print(data)
        sleep(.01)
    print('exiting read thread')

def main():
    global stop_threads
    global use_crc
    # prompt for comm parameters
    baudrate = input('baudrate (default is 115200): ')
    baudrate = int(baudrate) if baudrate else 115200
    
    port = input('Comm port (e.g./dev/ttyACM0(default) or /dev/ttyUSB0): or COM5 ')
    port = port if port else '/dev/ttyACM0'
    
    use_crc = input('Use CRC (y(default) or n): ')
    use_crc = False if use_crc.upper().startswith('N') else True

    print("\tbaudrate: " + str(baudrate) + "\n\tport: " + port + "\n\tuse_crc: " + str(use_crc))
    
    # setup and open serical port
    serial_port.baudrate = baudrate
    serial_port.port = port 
    serial_port.timeout = 0
    if serial_port.isOpen(): serial_port.close()
    serial_port.open()

    # start the read/write functions in concurent threads
    tread = threading.Thread(target=read, args=(), name='tread')
    twrite = threading.Thread(target=write, args=(), name='twrite')
    twrite.start()
    tread.start()
    while True:
        try:
            sleep(.5)
        except KeyboardInterrupt:
            #print('main(): received KeyboardInterrupt')
            stop_threads = True
            break

    tread.join()
    twrite.join()
    print('Threads have finished')
    serial_port.close()
    print('main() is done')

if(__name__ == '__main__'):
	main()
	print('All done')
