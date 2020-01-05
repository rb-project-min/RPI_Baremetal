#include <stddef.h>

#include "UART5.h"
#include "GPIO.h"
#include "mailbox.h"

uint32_t uart5_init(uint32_t Baud) {

// Step1. UART�̎��O�ݒ�
	// UART�̖�����
	UART5_CR = 0;

// Step 2: GPIO��UART��
	// GPIO �v���A�b�v�ݒ�
	GPIO_Pull(12, GPPUD_UP);
	GPIO_Pull(13, GPPUD_UP);

	// GPIO �t�@���N�V�����ݒ�
	GPIO_Mode(12, GPF_ALT4);
	GPIO_Mode(13, GPF_ALT4);

// Step 3: �{�[���[�g�ݒ�
	uint32_t Core = GetUartCurrentClock();
	uint32_t IBRD = Core / (16 * Baud);
	uint32_t FBRD = (Core * 8 / Baud - IBRD * 128 + 1) >> 1;

	UART5_IBRD = IBRD;
	UART5_FBRD = FBRD;

// Step 4: ���M ��M�̗L����
	UART5_ICR  = 0x7FF;     // ���荞�݂��N���A����

	// �f�[�^��8bit, FIFO�L��, �X�g�b�v�r�b�g 1bit, �p���e�B����
	UART5_LCRH = 0x70;

	// CTS/RTS����, ��M/���M�L��, ���[�v����, UART�L��
	UART5_CR = 0x0301;

	return 0;
}

// 1�o�C�g���M
void uart5_putc(char byte) {
	while (UART5_FR & (1U << 5));
	UART5_DR = (uint32_t)byte;
}

// ������(�I�[NULL)���M
void uart5_puts(const char *str) {
	while (*str) {
		uart5_putc(*str++);
	}
}

// 1�o�C�g��M
char uart5_getc() {
	while (UART5_FR & (1U << 4));
	return (char)(UART5_DR & 0xFF);
}

