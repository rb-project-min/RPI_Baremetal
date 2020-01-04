#include <stdint.h>
#include <stdbool.h>

extern void *RPi_IO_Base_Addr;
extern uint32_t CoreExecute(uint32_t CoreID, void *Address);

#define GPIO_BASE     (RPi_IO_Base_Addr + 0x00200000)

/* GPIO Function Select */
#define GPFSEL0 (*(volatile uint32_t*)(GPIO_BASE + 0x00))
#define GPFSEL1 (*(volatile uint32_t*)(GPIO_BASE + 0x04))
#define GPFSEL2 (*(volatile uint32_t*)(GPIO_BASE + 0x08))
#define GPFSEL3 (*(volatile uint32_t*)(GPIO_BASE + 0x0C))
#define GPFSEL4 (*(volatile uint32_t*)(GPIO_BASE + 0x10))
#define GPFSEL5 (*(volatile uint32_t*)(GPIO_BASE + 0x14))

/* GPIO Pin Output Set */
#define GPSET0  (*(volatile uint32_t*)(GPIO_BASE + 0x1C))
#define GPSET1  (*(volatile uint32_t*)(GPIO_BASE + 0x20))

/* GPIO Pin Output Clear */
#define GPCLR0  (*(volatile uint32_t*)(GPIO_BASE + 0x28))
#define GPCLR1  (*(volatile uint32_t*)(GPIO_BASE + 0x2C))

uint32_t main() {

	// GPIO47 出力に設定
	GPFSEL4 = (GPFSEL4 & ~(7 << (7 * 3))) | (1 << (7 * 3));
	// GPIO35 出力に設定
	GPFSEL3 = (GPFSEL3 & ~(7 << (5 * 3))) | (1 << (5 * 3));

	// セットして待つ、クリアして待つ、を繰り返す。
	while (1) {
		GPSET1 = 1 << 15;
		GPCLR1 = 1 << 3;
		for(volatile uint32_t i = 0; i < 3000000; i++);
		GPCLR1 = 1 << 15;
		GPSET1 = 1 << 3;
		for(volatile uint32_t i = 0; i < 3000000; i++);
	}

	return 0;
}
