/* Taken from https://github.com/vdudouyt/sdcc-examples-stm8.git
 * Modified for STM8L152R8T6 (www.st.com/stm8nucleo)
 * LD2 is a user LED connected to PB5
 * How to flash: stm8flash -c stlinkv21 -p stm8l152r8 -w blinky.ihx
 */

#include <stdbool.h>
#include "stm8l.h"

int main()
{
	PB_DDR = 0x20;		// Data Direction for PB5 is output
	PB_CR1 = 0x20;		// Control 1 push-pull for PB5

	while(true) {
		PB_ODR ^= 0x20;	// flip Output Data value for PB5
		for(volatile int d = 0; d < 29000; d++) { }	// Busy delay-loop
	}
}
