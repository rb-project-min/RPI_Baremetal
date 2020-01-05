#ifndef GPIO_H
#define GPIO_H
#include <stdint.h>
#include <stdbool.h>

#include "rpi.h"

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

/* GPIO Pin Level */
#define GPLEV0  (*(volatile uint32_t*)(GPIO_BASE + 0x34))
#define GPLEV1  (*(volatile uint32_t*)(GPIO_BASE + 0x38))

/* GPIO Pin Pull-up/down Enable */
#define GPPUD       (*(volatile uint32_t*)(GPIO_BASE + 0x94))

/* GPIO Pin Pull-up/down Enable Clock */
#define GPPUDCLK0   (*(volatile uint32_t*)(GPIO_BASE + 0x98))
#define GPPUDCLK1   (*(volatile uint32_t*)(GPIO_BASE + 0x9C))

/* GPIO Function */
#define GPF_INPUT  0
#define GPF_OUTPUT 1
#define GPF_ALT0   4
#define GPF_ALT1   5
#define GPF_ALT2   6
#define GPF_ALT3   7
#define GPF_ALT4   3
#define GPF_ALT5   2
#define GPF_MASK   7

/* PUD */
#define GPPUD_OFF  0
#define GPPUD_DOWN 1
#define GPPUD_UP   2
#define GPPUD_MASK 3

void GPIO_Mode(uint32_t pin, uint32_t mode);
void GPIO_Pull(uint32_t pin, uint32_t pull);
void GPIO_Write(uint32_t pin, bool value);
bool GPIO_Read(uint32_t pin);

#endif
