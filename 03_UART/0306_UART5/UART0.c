#include <stddef.h>

#include "UART0.h"
#include "GPIO.h"
#include "mailbox.h"

uint32_t uart0_init(uint32_t Baud) {

// Step1. UARTの事前設定
	// UARTの無効化
	UART0_CR = 0;

// Step 2: GPIOをUART化
	// GPIO プルアップ設定
	GPIO_Pull(14, GPPUD_UP);
	GPIO_Pull(15, GPPUD_UP);

	// GPIO ファンクション設定
	GPIO_Mode(14, GPF_ALT0);
	GPIO_Mode(15, GPF_ALT0);

// Step 3: ボーレート設定
	uint32_t Core = GetUartCurrentClock();
	uint32_t IBRD = Core / (16 * Baud);
	uint32_t FBRD = (Core * 8 / Baud - IBRD * 128 + 1) >> 1;

	UART0_IBRD = IBRD;
	UART0_FBRD = FBRD;

// Step 4: 送信 受信の有効化
	UART0_ICR  = 0x7FF;     // 割り込みをクリアする

	// データ長8bit, FIFO有効, ストップビット 1bit, パリティ無効
	UART0_LCRH = 0x70;

	// CTS/RTS無効, 受信/送信有効, ループ無効, UART有効
	UART0_CR = 0x0301;

	return 0;
}

// 1バイト送信
void uart0_putc(char byte) {
	while (UART0_FR & (1U << 5));
	UART0_DR = (uint32_t)byte;
}

// 文字列(終端NULL)送信
void uart0_puts(const char *str) {
	while (*str) {
		uart0_putc(*str++);
	}
}

// 1バイト受信
char uart0_getc() {
	while (UART0_FR & (1U << 4));
	return (char)(UART0_DR & 0xFF);
}

