#include <stdint.h>
#include <stdbool.h>

extern void *RPi_IO_Base_Addr;
uint32_t CoreExecute(uint32_t CoreID, void *Address);

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

uint32_t main1();
uint32_t main2();
uint32_t main3();

uint32_t main() {

	// GPIO16 出力に設定
	GPFSEL1 = (GPFSEL1 & ~(7 << (6 * 3))) | (1 << (6 * 3));

	CoreExecute(1, main1);
	for(volatile uint32_t i = 0; i < 3000000; i++);	//念のため遅延ループ
	CoreExecute(2, main2);
	for(volatile uint32_t i = 0; i < 3000000; i++);	//念のため遅延ループ
	CoreExecute(3, main3);
	for(volatile uint32_t i = 0; i < 3000000; i++);	//念のため遅延ループ
	
	// セットして待つ、クリアして待つ、を繰り返す。
	while (1) {
		GPSET0 = 1 << 16;
		for(volatile uint32_t i = 0; i < 3000000; i++);
		GPCLR0 = 1 << 16;
		for(volatile uint32_t i = 0; i < 3000000; i++);
	}

	return 0;
}

uint32_t main1() {

	// GPIO17 出力に設定
	GPFSEL1 = (GPFSEL1 & ~(7 << (7 * 3))) | (1 << (7 * 3));

	// セットして待つ、クリアして待つ、を繰り返す。
	while (1) {
		GPSET0 = 1 << 17;
		for(volatile uint32_t i = 0; i < 3000000; i++);
		GPCLR0 = 1 << 17;
		for(volatile uint32_t i = 0; i < 3000000; i++);
	}

	return 0;
}

uint32_t main2() {

	// GPIO18 出力に設定
	GPFSEL1 = (GPFSEL1 & ~(7 << (8 * 3))) | (1 << (8 * 3));

	// セットして待つ、クリアして待つ、を繰り返す。
	while (1) {
		GPSET0 = 1 << 18;
		for(volatile uint32_t i = 0; i < 3000000; i++);
		GPCLR0 = 1 << 18;
		for(volatile uint32_t i = 0; i < 3000000; i++);
	}

	return 0;
}

uint32_t main3() {

	// GPIO19 出力に設定
	GPFSEL1 = (GPFSEL1 & ~(7 << (9 * 3))) | (1 << (9 * 3));

	// セットして待つ、クリアして待つ、を繰り返す。
	while (1) {
		GPSET0 = 1 << 19;
		for(volatile uint32_t i = 0; i < 3000000; i++);
		GPCLR0 = 1 << 19;
		for(volatile uint32_t i = 0; i < 3000000; i++);
	}

	return 0;
}
