#include <stddef.h>

#include "UART.h"
#include "GPIO.h"
#include "mailbox.h"

uint32_t uart_init(uint32_t id, uint32_t Baud) {
	if(id > 5) return 1;
	if(id > 1 && (uint32_t)RPi_IO_Base_Addr < 0x80000000) return 1;

	void *UART_BASE = RPi_IO_Base_Addr + 0x00201000 + id * 0x200;

// Step1. UARTの事前設定
	if(id == 1) {
		// UART1の有効化
		AUX_ENABLES |= 0x00000001;
		// 送信 受信の無効化
		UART1_CNTL = 0x00000000;
	}
	else {
		// UARTの無効化
		UART_CR = 0;
	}

// Step 2: GPIOをUART化
	switch(id) {
		case 0:
			GPIO_Pull(14, GPPUD_UP);
			GPIO_Pull(15, GPPUD_UP);
			GPIO_Mode(14, GPF_ALT0);
			GPIO_Mode(15, GPF_ALT0);
			break;
		case 1:
			GPIO_Pull(14, GPPUD_UP);
			GPIO_Pull(15, GPPUD_UP);
			GPIO_Mode(14, GPF_ALT5);
			GPIO_Mode(15, GPF_ALT5);
			break;
		case 2:
			GPIO_Pull(0, GPPUD_UP);
			GPIO_Pull(1, GPPUD_UP);
			GPIO_Mode(0, GPF_ALT4);
			GPIO_Mode(1, GPF_ALT4);
			break;
		case 3:
			GPIO_Pull(4, GPPUD_UP);
			GPIO_Pull(5, GPPUD_UP);
			GPIO_Mode(4, GPF_ALT4);
			GPIO_Mode(5, GPF_ALT4);
			break;
		case 4:
			GPIO_Pull(8, GPPUD_UP);
			GPIO_Pull(9, GPPUD_UP);
			GPIO_Mode(8, GPF_ALT4);
			GPIO_Mode(9, GPF_ALT4);
			break;
		case 5:
			GPIO_Pull(12, GPPUD_UP);
			GPIO_Pull(13, GPPUD_UP);
			GPIO_Mode(12, GPF_ALT4);
			GPIO_Mode(13, GPF_ALT4);
			break;
		default:
			return 1;
	}
// Step 3: ボーレート設定
	if(id == 1) {
		UART1_BAUD = (GetCoreCurrentClock() / (4 * Baud) - 1) >> 1;
	}
	else {
		uint32_t Core = GetUartCurrentClock();
		uint32_t IBRD = Core / (16 * Baud);
		uint32_t FBRD = (Core * 8 / Baud - IBRD * 128 + 1) >> 1;

		UART_IBRD = IBRD;
		UART_FBRD = FBRD;
	}

// Step 4: 送信 受信の有効化
	if(id == 1) {
		// 割り込みをクリアする
		UART1_IER = 0x00000000;

		// 8bit通信を有効にする．
		UART1_LCR = 0x00000003;

		// 全ての割り込みを有効に 
		UART1_IER = 0x00000003;

		// FIFOをクリア
		UART1_IIR = 0x000000C7;

		// 送信 受信の有効化
		UART1_CNTL = 0x00000003;
	}
	else {
		UART_ICR  = 0x7FF;     // 割り込みをクリアする

		// データ長8bit, FIFO有効, ストップビット 1bit, パリティ無効
		UART_LCRH = 0x70;

		// CTS/RTS無効, 受信/送信有効, ループ無効, UART有効
		UART_CR = 0x0301;
	}
	return 0;
}

// 1バイト送信
void uart_putc(uint32_t id, char byte) {
	if(id > 5) return;
	if(id > 1 && (uint32_t)RPi_IO_Base_Addr < 0x80000000) return;

	if(id == 1) {
		while (!(UART1_LSR & (1U << 6)) && !(UART1_LSR & (1U << 5)));
		UART1_IO = (uint32_t)byte;
	}
	else {
		void *UART_BASE = RPi_IO_Base_Addr + 0x00201000 + id * 0x200;
		while (UART_FR & (1U << 5));
		UART_DR = (uint32_t)byte;
	}
}

// 文字列(終端NULL)送信
void uart_puts(uint32_t id, const char *str) {
	if(id > 5) return;
	if(id > 1 && (uint32_t)RPi_IO_Base_Addr < 0x80000000) return;

	while (*str) {
		uart_putc(id, *str++);
	}
}

// 1バイト受信
char uart_getc(uint32_t id) {
	if(id > 5) return '\0';
	if(id > 1 && (uint32_t)RPi_IO_Base_Addr < 0x80000000) return '\0';

	if(id == 1) {
		while (!(UART1_LSR & (1U << 0)));
		return UART1_IO;
	}
	else {
		void *UART_BASE = RPi_IO_Base_Addr + 0x00201000 + id * 0x200;
		while (UART_FR & (1U << 4));
		return (char)(UART_DR & 0xFF);
	}
}

