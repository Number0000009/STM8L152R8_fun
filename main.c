/*
 * There's a lot of examples on the Internet, but this one actually works
 * on my STM8L152R8 board.
 * sdcc -lstm8 -mstm8 --out-fmt-ihx main.c
 */

#include <stdint.h>

#define F_CPU 2000000UL	// Default is 2MHz

#define MMIO(mem_addr)	(*(volatile uint8_t *)(0x5000 + (mem_addr)))

// Port C
#define PC_ODR		MMIO(0x0A)
#define PC_DDR		MMIO(0x0C)
#define PC_CR1		MMIO(0x0D)
#define PC_CR2		MMIO(0x0E)
#define USART_TX_PIN	3

// Port D
#define PD_ODR		MMIO(0x0F)
#define PD_DDR		MMIO(0x11)
#define PD_CR1		MMIO(0x12)
#define PD4_PIN		4

// USART1
#define USART1_SR	MMIO(0x230)
#define USART1_DR	MMIO(0x231)
#define USART1_BRR1	MMIO(0x232)
#define USART1_BRR2	MMIO(0x233)
#define USART1_CR1	MMIO(0x234)
#define USART1_CR2	MMIO(0x235)
#define USART1_CR3	MMIO(0x236)
#define USART_TXE	7
#define USART_TC	6
#define USART_RXNE	5
#define USART_TEN	3
#define USART_REN	2

#define USART_STOP1	4
#define USART_STOP2	5

#define CLK_DIVR	MMIO(0x0c0)
#define CLK_PCKENR1	MMIO(0x0c3)
#define PCKEN_USART1	5


static inline void delay_ms(uint16_t ms)
{
// The fuck sdcc -std-c99 doesn't allow
// defining variables withing the for-clause???
	uint32_t i;

	for (i = 0; i < ((F_CPU / 18000UL) * ms); i++)
		__asm__("nop");
}

void main()
{
//	CLK_DIVR = 0;				// not divided == 16MHz
	CLK_PCKENR1 = (1 << PCKEN_USART1);	// enable clock to USART1

	PD_DDR |= (1 << PD4_PIN);		// output
	PD_CR1 |= (1 << PD4_PIN);		// push-pull mode

	PC_DDR |= (1 << USART_TX_PIN);		// output
	PC_CR1 |= (1 << USART_TX_PIN);		// push-pull mode


//	USART1_BRR2 = 0x03;			//
//	USART1_BRR1 = 0x68;			// 9600 @ 16Mhz
//	USART1_BRR2 = 0x00;			// reset value is 0
	USART1_BRR1 = 0x0D;			// 9600 @ 2Mhz

//	USART1_CR3 &= ~(1 << USART_STOP1) | (1 << USART_STOP2);	// 00: 1 STOP bit
//								// reset value is already 0, anyway
	USART1_CR2 = (1 << USART_TEN) | (1 << USART_REN);	// exnable TX and RX

	while (1)
	{
		if (USART1_SR & (1 << USART_TXE))
			USART1_DR = 'A';

		PD_ODR ^= (1 << PD4_PIN);	// toggle PD4-pin
		delay_ms(250);
	}


/*
// the end
	while (!(USART1_SR & (1 << USART_TC))) {
	}

	__asm__("halt");
*/
}

