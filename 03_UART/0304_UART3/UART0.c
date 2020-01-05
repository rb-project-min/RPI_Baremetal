#include <stddef.h>

#include "UART0.h"
#include "GPIO.h"
#include "mailbox.h"

uint32_t uart0_init(uint32_t Baud) {

// Step1. UART�̎��O�ݒ�
	// UART�̖�����
	UART0_CR = 0;

// Step 2: GPIO��UART��
	// GPIO �v���A�b�v�ݒ�
	GPIO_Pull(14, GPPUD_UP);
	GPIO_Pull(15, GPPUD_UP);

	// GPIO �t�@���N�V�����ݒ�
	GPIO_Mode(14, GPF_ALT0);
	GPIO_Mode(15, GPF_ALT0);

// Step 3: �{�[���[�g�ݒ�
	uint32_t Core = GetUartCurrentClock();
	uint32_t IBRD = Core / (16 * Baud);
	uint32_t FBRD = (Core * 8 / Baud - IBRD * 128 + 1) >> 1;

	UART0_IBRD = IBRD;
	UART0_FBRD = FBRD;

// Step 4: ���M ��M�̗L����
	UART0_ICR  = 0x7FF;     // ���荞�݂��N���A����

	// �f�[�^��8bit, FIFO�L��, �X�g�b�v�r�b�g 1bit, �p���e�B����
	UART0_LCRH = 0x70;

	// CTS/RTS����, ��M/���M�L��, ���[�v����, UART�L��
	UART0_CR = 0x0301;

	return 0;
}

// 1�o�C�g���M
void uart0_putc(char byte) {
	while (UART0_FR & (1U << 5));
	UART0_DR = (uint32_t)byte;
}

// ������(�I�[NULL)���M
void uart0_puts(const char *str) {
	while (*str) {
		uart0_putc(*str++);
	}
}

// 1�o�C�g��M
char uart0_getc() {
	while (UART0_FR & (1U << 4));
	return (char)(UART0_DR & 0xFF);
}

