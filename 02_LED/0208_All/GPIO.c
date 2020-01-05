#include "GPIO.h"

void GPIO_Mode(uint32_t pin, uint32_t mode) {
	uint32_t addr = (pin % 10) * 3;
	uint32_t reg = pin / 10;
	switch(reg)
	{
		case 0:
			GPFSEL0 = (GPFSEL0 & ~(GPF_MASK << addr)) | (mode << addr);
			return;
		case 1:
			GPFSEL1 = (GPFSEL1 & ~(GPF_MASK << addr)) | (mode << addr);
			return;
		case 2:
			GPFSEL2 = (GPFSEL2 & ~(GPF_MASK << addr)) | (mode << addr);
			return;
		case 3:
			GPFSEL3 = (GPFSEL3 & ~(GPF_MASK << addr)) | (mode << addr);
			return;
		case 4:
			GPFSEL4 = (GPFSEL4 & ~(GPF_MASK << addr)) | (mode << addr);
			return;
		default:
			GPFSEL5 = (GPFSEL5 & ~(GPF_MASK << addr)) | (mode << addr);
			return;
	}
}

void GPIO_Write(uint32_t pin, bool value)
{
	if (value) {
		if (pin < 32) GPSET0 = 1 << pin;
		else GPSET1 = 1 << (pin - 32);
	} else {
		if (pin < 32) GPCLR0 = 1 << pin;
		else GPCLR1 = 1 << (pin - 32);
	}
}

bool GPIO_Read(uint32_t pin)
{
	if (pin < 32) return (GPLEV0 & (1 << pin)) != 0;
	else return (GPLEV1 & (1 << (pin - 32))) != 0;
}
