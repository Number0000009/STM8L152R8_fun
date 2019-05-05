/*
 * STM8L152R8 board + CSR H13467V3 shield
 * sdcc -lstm8 -mstm8 --out-fmt-ihx main.c
 */

#include <stdint.h>

#define F_CPU 2000000UL	// Default is 2MHz

#define MMIO(mem_addr)	(*(volatile uint8_t *)(0x5000 + (mem_addr)))
/*
H13467 (ORG4572)	STM8L152R8 
TX			PE0 (D2)
WAK			PE1 (D3)
RST			PE2 (D4)
BTN			PC2 (D5)
LD2			PC3 (D6)
LD1			PD1 (D7)
RX			PD3 (D8)
ON			PC4 (D9)
OSP 115200
*/

// Port C
#define PC_ODR		MMIO(0x0A)
#define PC_DDR		MMIO(0x0C)
#define PC_CR1		MMIO(0x0D)
#define PC_CR2		MMIO(0x0E)
#define USART_TX_PIN	3
#define	PC_LD2_PIN	3

// Port D
#define PD_ODR		MMIO(0x0F)
#define PD_DDR		MMIO(0x11)
#define PD_CR1		MMIO(0x12)
#define PD_LD1_PIN	1
#define PD_RX_PIN	3

// Port E
#define PE_ODR		MMIO(0x14)
#define PE_DDR		MMIO(0x16)
#define PE_CR1		MMIO(0x17)
#define PE4_PIN		4

// USART1
#define USART1_SR	MMIO(0x230)
#define USART1_DR	MMIO(0x231)
#define USART1_BRR1	MMIO(0x232)
#define USART1_BRR2	MMIO(0x233)
#define USART1_CR1	MMIO(0x234)
#define USART1_CR2	MMIO(0x235)
#define USART1_CR3	MMIO(0x236)

// USART3
#define USART3_SR	MMIO(0x3f0)
#define USART3_DR	MMIO(0x3f1)
#define USART3_BRR1	MMIO(0x3f2)
#define USART3_BRR2	MMIO(0x3f3)
#define USART3_CR1	MMIO(0x3f4)
#define USART3_CR2	MMIO(0x3f5)
#define USART3_CR3	MMIO(0x3f6)

#define USART_TXE	7
#define USART_TC	6
#define USART_RXNE	5
#define USART_TEN	3
#define USART_REN	2

#define USART_STOP1	4
#define USART_STOP2	5

#define CLK_DIVR	MMIO(0x0c0)
#define CLK_PCKENR1	MMIO(0x0c3)
#define CLK_PCKENR3	MMIO(0x0d0)
#define PCKEN1_USART1	5
#define PCKEN3_USART3	4


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
	CLK_PCKENR3 = (1 << PCKEN3_USART3);	// enable clock to USART3

	PD_DDR |= (1 << PD_LD1_PIN);		// output
	PD_CR1 |= (1 << PD_LD1_PIN);		// push-pull mode

	PC_DDR |= (1 << PC_LD2_PIN);		// output
	PC_CR1 |= (1 << PC_LD2_PIN);		// push-pull mode

	PD_ODR |= (1 << PD_LD1_PIN);		// set LD1-pin
	PC_ODR &= ~(1 << PC_LD2_PIN);		// reset LD2-pin

//	USART3_BRR2 = 0x03;			//
//	USART3_BRR1 = 0x68;			// 9600 @ 16Mhz
	USART3_BRR2 = 0x00;			// reset value is 0
	USART3_BRR1 = 0x0D;			// 9600 @ 2Mhz

	delay_ms(1);				// for 9600Hz

//	USART3_CR3 &= ~(1 << USART_STOP1) | (1 << USART_STOP2);	// 00: 1 STOP bit
//								// reset value is already 0, anyway
	USART3_CR2 = (1 << USART_TEN) | (1 << USART_REN);	// exnable TX and RX

	while (1)
	{
		if (USART3_SR & (1 << USART_TXE))
			USART3_DR = 'A';

		PC_ODR ^= (1 << PC_LD2_PIN);	// toggle LD2-pin
		PD_ODR ^= (1 << PD_LD1_PIN);	// toggle LD1-pin

		delay_ms(250);
	}


/*
// the end
	while (!(USART1_SR & (1 << USART_TC))) {
	}

	__asm__("halt");
*/
}

