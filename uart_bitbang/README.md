# Bit-banging UART TX on PD4

## TODO
- _delay_ms busy-loop sucks, this should use timer
- sdcc generates quite a shitty code, so resulting bit-banged pattern has quite
a bit of a jitter, usually it's 9100 baud instead of 9600. Probably raising
clock freq would help
