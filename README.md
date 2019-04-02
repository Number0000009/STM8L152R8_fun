# Fun with STM8L152R8

## What it does:

Minimalistic C program that initializes GPIO for PD4 and USART1_TX pins,
enables USART1 clock, initializes USART1 as 9600 8N1.
When running it flips PD4 pin and prints 'A' to USART1.

![alt text](https://github.com/Number0000009/STM8L152R8_fun/blob/master/pictures/Capture.JPG "Capture")
![alt text](https://github.com/Number0000009/STM8L152R8_fun/blob/master/pictures/Capture1.JPG "Capture zoomed in")

## How to build

sdcc -lstm8 -mstm8 --out-fmt-ihx main.c

## How to program

Use your favourite method, I was using ST Visual Programmer in Windows 10

## How do I probe PD4 and USART1_TX pins?

Logic analyzer? PD4 can be probed with an LED and a resistor?
I used an Open Bench Logic Sniffer from Dangerous Prototypes, although I cannot
recommend it.

## Manuals

I used these:

_DocID15226 Rev 14_ Describes registers bits

_DS6948 Rev 11_ Describes memory map and mmio addresses

_UM2351 Rev 1_ Describes pinouts and schematics

## How to fix STVD in Windows 10

run cmd in administrator mode

	regsvr32 /u "C:\Program Files (x86)\Common Files\Microsoft Shared\DAO\DAO350.DLL"
	regsvr32 "C:\Program Files (x86)\Common Files\Microsoft Shared\DAO\DAO350.DLL"
