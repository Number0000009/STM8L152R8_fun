## What this is:

Read thermistor TTF-103 and print readings to UART1 for stm8nucleo board www.st.com/stm8nucleo

## What it does:

Reads thermistor value using A1 (ADC_IN0 aka PA6) and prints reading to UART1 (PC 3)

## How to build

You need SDCC and then:

`make`

or

`sdcc -lstm8 -mstm8 --out-fmt-ihx temp.c`

## How to program

`stm8flash -c stlinkv21 -p stm8l152r8 -w temp.ihx`

## How to run

Attach the following:
```
	-+- Vcc 3.3V
         |
	 \
	 /  TTF-103
	 \
	 |
ADC_IN0 -+
	 |
	 \
	 /  R0 = 10 KOhm
	 \
	 |
	-+- GND
```
Attach to /dev/ttyUSB at 9600 8N1 no HW-flow control, no SW-flow control to read ADC values.

Or

run 'read.py' for continuos reading and conversion to T°C
