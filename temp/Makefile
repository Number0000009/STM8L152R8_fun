CC=sdcc
OBJ=temp.ihx

.PHONY: all clean

all: $(OBJ)

clean:
	rm -f $(OBJ)

%.ihx: %.c
	$(CC) -lstm8 -mstm8 --out-fmt-ihx $(CFLAGS) $(LDFLAGS) $<
