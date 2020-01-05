#include <stddef.h>

#include "UART4.h"
#include "GPIO.h"
#include "mailbox.h"

uint32_t uart4_init(uint32_t Baud) {

// Step1. UART�̎��O�ݒ�
	// UART�̖�����
	UART4_CR = 0;

// Step 2: GPIO��UART��
	// GPIO �v���A�b�v�ݒ�
	GPIO_Pull(8, GPPUD_UP);
	GPIO_Pull(9, GPPUD_UP);

	// GPIO �t�@���N�V�����ݒ�
	GPIO_Mode(8, GPF_ALT4);
	GPIO_Mode(9, GPF_ALT4);

// Step 3: �{�[���[�g�ݒ�
	uint32_t Core = GetUartCurrentClock();
	uint32_t IBRD = Core / (16 * Baud);
	uint32_t FBRD = (Core * 8 / Baud - IBRD * 128 + 1) >> 1;

	UART4_IBRD = IBRD;
	UART4_FBRD = FBRD;

// Step 4: ���M ��M�̗L����
	UART4_ICR  = 0x7FF;     // ���荞�݂��N���A����

	// �f�[�^��8bit, FIFO�L��, �X�g�b�v�r�b�g 1bit, �p���e�B����
	UART4_LCRH = 0x70;

	// CTS/RTS����, ��M/���M�L��, ���[�v����, UART�L��
	UART4_CR = 0x0301;

	return 0;
}

// 1�o�C�g���M
void uart4_putc(char byte) {
	while (UART4_FR & (1U << 5));
	UART4_DR = (uint32_t)byte;
}

// ������(�I�[NULL)���M
void uart4_puts(const char *str) {
	while (*str) {
		uart4_putc(*str++);
	}
}

// 1�o�C�g��M
char uart4_getc() {
	while (UART4_FR & (1U << 4));
	return (char)(UART4_DR & 0xFF);
}

