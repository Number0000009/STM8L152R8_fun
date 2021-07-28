#include <stdint.h>
#include <stdbool.h>

// ST-Link v2.1 VCOM UART is 115200

#define F_CPU 16000000UL	// 16MHz

#define MMIO(mem_addr)	(*(volatile uint8_t *)(0x5000 + (mem_addr)))

// Port E
#define PE_ODR		MMIO(0x14)
#define PE_DDR		MMIO(0x16)
#define PE_CR1		MMIO(0x17)
#define PE3_PIN		3
#define PE4_PIN		4

#define CLK_DIVR	MMIO(0x0c0)
#define CLK_PCKENR1	MMIO(0x0c3)
#define PCKEN_TIM4	2

#define TIM4_CR1	MMIO(0x2e0)
#define TIM4_IER	MMIO(0x2e4)
#define TIM4_SR		MMIO(0x2e5)
#define TIM4_EGR	MMIO(0x2e6)
#define TIM4_PSCR	MMIO(0x2e8)
#define TIM4_ARR	MMIO(0x2e9)

#define TIM4_CEN	0
#define TIM4_TIE	6
#define TIM4_UIE	0
#define TIM4_TG		6
#define TIM4_UG		0

// Vectors
#define TIM4_ISR 25	// int25

static inline void delay_ms(uint16_t ms)
{
	for (uint32_t i = 0; i < ((F_CPU / 18000UL) * ms); i++)
		__asm__("nop");
}

static inline void set_pe4_pin_high()
{
	PE_ODR |= 1 << PE4_PIN;
}

static inline void set_pe4_pin_low()
{
	PE_ODR &= ~(1 << PE4_PIN);
}

static inline void setup()
{
	CLK_DIVR = 0;				// not divided == 16MHz

	PE_DDR |= 1 << PE4_PIN;			// output
	PE_CR1 |= 1 << PE4_PIN;			// push-pull mode

	delay_ms(1);

	set_pe4_pin_high();
}

static inline void setup_timer4()
{
// enable clock to TIM4 (basic 8-bit timer)
	CLK_PCKENR1 |= 1 << PCKEN_TIM4;

	TIM4_ARR = 138;				// 115200Hz (F_CPU/115200)

	TIM4_IER |= 1 << TIM4_UIE;
	TIM4_EGR |= 1 << TIM4_UG;

	__asm__("rim");
}

static inline int bitbang(uint8_t c)
{
	static int i = -1;

	if (i == -1) {
// 1 start bit
		set_pe4_pin_low();
		i++;
		return false;
	}

	if (c & (1 << i))
		set_pe4_pin_high();
	else
		set_pe4_pin_low();

	i++;
	if (i == 9) {
		set_pe4_pin_high();

		TIM4_CR1 &= ~(1 << TIM4_CEN);	// disable TIM4
		i = -1;

		return true;
	}

	return false;
}

void trap_isr() __trap
{
	__asm__("jra .");
}

char str[] = {"Hello world\n\r"};


void tim4_isr() __interrupt(TIM4_ISR)
{
	static int z = 0;

	if (str[z]) {
		int ret = bitbang(str[z]);
		if (ret) {
			z++;
		}
	} else {
		z = 0;
	}
	TIM4_SR = 0;
}

void main()
{
	static int z = 0;

	setup();
	setup_timer4();

	while (1)
	{
		TIM4_CR1 |= 1 << TIM4_CEN;		// enable TIM4

		delay_ms(250);
	}
}

