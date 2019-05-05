# Fun with CSR H13467 v3 shield

## What it does:

Nothing yet.

## How to build

sdcc -lstm8 -mstm8 --out-fmt-ihx main.c

## How to program

Use your favourite method, I was using ST Visual Programmer in Windows 10

## Manuals

I used these:

_DocID15226 Rev 14_ Describes registers bits

_DS6948 Rev 11_ Describes memory map and mmio addresses

_UM2351 Rev 1_ Describes pinouts and schematics

## How to fix STVD in Windows 10

run cmd in administrator mode

	regsvr32 /u "C:\Program Files (x86)\Common Files\Microsoft Shared\DAO\DAO350.DLL"
	regsvr32 "C:\Program Files (x86)\Common Files\Microsoft Shared\DAO\DAO350.DLL"
