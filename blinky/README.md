## What this is:
Blinkenlight for STM8L152R8 (stm8nucleo board) www.st.com/stm8nucleo

[Original](https://github.com/vdudouyt/sdcc-examples-stm8.git)

Just a minimal Hello-world blinky-style.

## What it does:

Blinks LD2.

## How to build

You need SDCC and then:

`make`

or

`sdcc -lstm8 -mstm8 --out-fmt-ihx blinky.c`

## How to program

`stm8flash -c stlinkv21 -p stm8l152r8 -w blinky.ihx`
