#include <stddef.h>

#include "uart1.h"
#include "GPIO.h"
#include "mailbox.h"

uint32_t uart1_init(uint32_t Baud) {

// Step1. UARTの事前設定
	// UART1の有効化
	AUX_ENABLES |= 0x00000001;
	// 送信 受信の無効化
	UART1_CNTL = 0x00000000;

// Step 2: GPIOをUART化
	// GPIO プルアップ設定
	GPIO_Pull(14, GPPUD_UP);
	GPIO_Pull(15, GPPUD_UP);

	// GPIO ファンクション設定
	GPIO_Mode(14, GPF_ALT5);
	GPIO_Mode(15, GPF_ALT5);

// Step 3: ボーレート設定
	UART1_BAUD = (GetCoreCurrentClock() / (4 * Baud) - 1) >> 1;

// Step 4: 送信 受信の有効化
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

	return 0;
}

// 1バイト送信
void uart1_putc(char byte) {
	while (!(UART1_LSR & (1U << 6)) && !(UART1_LSR & (1U << 5)));
	UART1_IO = (uint32_t)byte;
}

// 文字列(終端NULL)送信
void uart1_puts(const char *str) {
	while (*str) {
		uart1_putc(*str++);
	}
}

// 1バイト受信
char uart1_getc() {
	while (!(UART1_LSR & (1U << 0)));
	return UART1_IO;
}

