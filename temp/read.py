#!/usr/bin/env python3

import serial
import math

def conv(ADC_val = 2125):
	err = 0.5		# wtf?
	RES = 2 ** 12		# 12-bits precision
	Vcc = 3.3		# 5V or 3.3V
	R0 = 10000
	B = 3435		# From TTF 103 datasheet
	Ka = 273.15
	T2 = Ka + 25

	val = ADC_val * (Vcc/RES)

	print("val = ", val)

	Rt = (Vcc - val) * R0/val

	print("Rt = ", Rt)

	temp = 1/(1/T2+math.log(R0/Rt)/B)-Ka+err

	print("T°C = ", temp)


ser = serial.Serial("/dev/ttyUSB0", baudrate = 9600, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, bytesize=serial.EIGHTBITS, timeout=1)

val = 0

while True:
	data = ser.read()
	if data == b'\n':
		val = 0

		for i in range(4):
			data = ser.read()
			val = 10*val + int(data)

		conv(val)
