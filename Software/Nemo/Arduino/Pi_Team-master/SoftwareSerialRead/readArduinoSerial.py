#!/usr/bin/env python

import time
import serial

ser = serial.Serial(

    port ='/dev/ttyACM0',
    baudrate = 115200,
    parity = serial.PARITY_NONE,
    stopbits = serial.STOPBITS_ONE,
    bytesize = serial.EIGHTBITS,
    timeout = 1

)

while True:
    x = ser.readline().strip()
    if( x ):
        print( x )
