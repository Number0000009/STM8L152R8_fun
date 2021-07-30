#!/usr/bin/env python3

import math

err = 0.5		# wtf?
RES = 2 ** 12		# 12-bits precision
Vcc = 3.3		# 5V or 3.3V
ADC_val = 2125
R0 = 10000
B = 3435		# From TTF 103 datasheet
Ka = 273.15
T2 = Ka + 25

val = ADC_val * (Vcc/RES)

print("val = ", val)

Rt = (Vcc - val) * R0/val

print("Rt = ", Rt)

temp = 1/(1/T2+math.log(R0/Rt)/B)-Ka+err

print("T = ", temp)
