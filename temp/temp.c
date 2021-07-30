#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define F_CPU 2000000UL	// 2MHz

#define MMIO(mem_addr)	(*(volatile uint8_t *)(0x5000 + (mem_addr)))

// Port C
#define PC_ODR		MMIO(0x0A)
#define PC_DDR		MMIO(0x0C)
#define PC_CR1		MMIO(0x0D)
#define PC_CR2		MMIO(0x0E)
#define USART_TX_PIN	3

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
#define CLK_PCKENR2	MMIO(0x0c4)
#define PCKEN1_USART1	5
#define PCKEN1_TIM4	2
#define PCKEN2_ADC1	0

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

#define ADC_CR1		MMIO(0x340)
#define ADC_CR2		MMIO(0x341)
#define ADC_CR3		MMIO(0x342)
#define	ADC_SR		MMIO(0x343)
#define ADC_DRH		MMIO(0x344)
#define ADC_DRL		MMIO(0x345)
#define ADC_HTRH	MMIO(0x346)
#define	ADC_HTRL	MMIO(0x347)
#define ADC_LTRH	MMIO(0x348)
#define ADC_LTRL	MMIO(0x349)
#define ADC_SQR1	MMIO(0x34a)
#define ADC_SQR2	MMIO(0x34b)
#define ADC_SQR3	MMIO(0x34c)
#define ADC_SQR4	MMIO(0x34d)
#define	ADC_TRIGR1	MMIO(0x34e)
#define ADC_TRIGR2	MMIO(0x34f)
#define ADC_TRIGR3	MMIO(0x350)
#define ADC_TRIGR4	MMIO(0x351)

#define ADC_ADON		0
#define ADC_START		1
#define ADC_CONT		2
#define ADC_EOC			0
#define ADC_SQR4_CH0		0
#define ADC_TRIGR4_SH_CH0	0
#define ADC_CR3_CHSEL_CH0	0

#define RI_IOSR1	MMIO(0x439)
#define RI_IOSR1_ADC1	0

// Vectors
#define TIM4_ISR 25	// int25

static inline void delay_ms(uint16_t ms)
{
	for (uint32_t i = 0; i < ((F_CPU / 18000UL) * ms); i++)
		__asm__("nop");
}

static inline void setup()
{
//	CLK_DIVR = 0;				// not divided == 16MHz
	CLK_PCKENR1 = 1 << PCKEN1_USART1;	// enable clock to USART1

	PC_DDR |= 1 << USART_TX_PIN;		// output
	PC_CR1 |= 1 << USART_TX_PIN;		// push-pull mode

	USART1_BRR1 = 0x0D;			// 9600 @ 2Mhz

// enable clock to ADC1
	CLK_PCKENR2 |= 1 << PCKEN2_ADC1;

// Route ADC1
	RI_IOSR1 |= 1 << RI_IOSR1_ADC1;

	delay_ms(1);

	USART1_CR2 = (1 << USART_TEN) | (1 << USART_REN);	// exnable TX and RX
}

static inline void setup_timer4()
{
// enable clock to TIM4 (basic 8-bit timer)
	CLK_PCKENR1 |= 1 << PCKEN1_TIM4;

	TIM4_ARR = 138;						// 8-bit TIM4 works at F_CPU rate, so 0xff-138 period

	TIM4_IER |= 1 << TIM4_UIE;
	TIM4_EGR |= 1 << TIM4_UG;

	__asm__("rim");
}

static inline void setup_adc()
{
	// ADC power on
	ADC_CR1 |= 1 << ADC_ADON;

	// ADC channel 0
	ADC_CR3 = 0xe0;

	ADC_SQR4 |= (1 << ADC_SQR4_CH0);

	// Shmitt trigger disabled
	ADC_TRIGR4 |= (1 << ADC_TRIGR4_SH_CH0);
}

void trap_isr() __trap
{
	__asm__("jra .");
}

void tim4_isr() __interrupt(TIM4_ISR)
{
	TIM4_SR = 0;
}

char str[] = {"Hello world\n\r"};

void main()
{
	static int z = 0;
	static int x = 0;

	bool skip = false;

	int adc_res = 0;

	static char str1[] = {"\r\n"};

	setup();
	setup_timer4();

	setup_adc();

	delay_ms(10);	// twkup

	while (1)
	{
		if (skip) {
			// ADC start conversion
			ADC_CR1 |= 1 << ADC_START;

			// Wait for End Of Conversion
			while (!(ADC_SR & (1 << ADC_EOC))) {}

			// Read data
			adc_res = (ADC_DRH << 8) | ADC_DRL;

			// Flip EOC
			ADC_SR &= ~ADC_EOC;

			_uitoa(adc_res, str, 10);
		}

		if (!skip) {
			if (USART1_SR & (1 << USART_TXE)) {
				if (str[z]) {
					USART1_DR = str[z++];
				} else {
					z = 0;
					skip = true;
				}
			}
		}

		if (skip) {
			if (USART1_SR & (1 << USART_TXE)) {
				if (str1[x]) {
					USART1_DR = str1[x++];
				} else {
					x = 0;
					skip = false;
				}
			}
		}

		TIM4_CR1 |= 1 << TIM4_CEN;		// enable TIM4

		delay_ms(250);
	}
}

