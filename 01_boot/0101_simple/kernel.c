#include <stdint.h>
#include <stdbool.h>

/* GPIO Function Select */
#define GPFSEL0 (*(volatile uint32_t*)(0x20200000 + 0x00))
#define GPFSEL1 (*(volatile uint32_t*)(0x20200000 + 0x04))
#define GPFSEL2 (*(volatile uint32_t*)(0x20200000 + 0x08))
#define GPFSEL3 (*(volatile uint32_t*)(0x20200000 + 0x0C))
#define GPFSEL4 (*(volatile uint32_t*)(0x20200000 + 0x10))
#define GPFSEL5 (*(volatile uint32_t*)(0x20200000 + 0x14))

/* GPIO Pin Output Set */
#define GPSET0  (*(volatile uint32_t*)(0x20200000 + 0x1C))
#define GPSET1  (*(volatile uint32_t*)(0x20200000 + 0x20))

/* GPIO Pin Output Clear */
#define GPCLR0  (*(volatile uint32_t*)(0x20200000 + 0x28))
#define GPCLR1  (*(volatile uint32_t*)(0x20200000 + 0x2C))

uint32_t main() {

	// GPIO16 出力に設定
	GPFSEL1 = (GPFSEL1 & ~(7 << (6 * 3))) | (1 << (6 * 3));

	// セットして待つ、クリアして待つ、を繰り返す。
	while (1) {
		GPSET0 = 1 << 16;
		for(volatile uint32_t i = 0; i < 3000000; i++);
		GPCLR0 = 1 << 16;
		for(volatile uint32_t i = 0; i < 3000000; i++);
	}

	return 0;
}

