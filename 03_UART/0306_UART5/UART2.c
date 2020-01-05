#include <stddef.h>

#include "UART2.h"
#include "GPIO.h"
#include "mailbox.h"

uint32_t uart2_init(uint32_t Baud) {

// Step1. UART�̎��O�ݒ�
	// UART�̖�����
	UART2_CR = 0;

// Step 2: GPIO��UART��
	// GPIO �v���A�b�v�ݒ�
	GPIO_Pull(0, GPPUD_UP);
	GPIO_Pull(1, GPPUD_UP);

	// GPIO �t�@���N�V�����ݒ�
	GPIO_Mode(0, GPF_ALT4);
	GPIO_Mode(1, GPF_ALT4);

// Step 3: �{�[���[�g�ݒ�
	uint32_t Core = GetUartCurrentClock();
	uint32_t IBRD = Core / (16 * Baud);
	uint32_t FBRD = (Core * 8 / Baud - IBRD * 128 + 1) >> 1;

	UART2_IBRD = IBRD;
	UART2_FBRD = FBRD;

// Step 4: ���M ��M�̗L����
	UART2_ICR  = 0x7FF;     // ���荞�݂��N���A����

	// �f�[�^��8bit, FIFO�L��, �X�g�b�v�r�b�g 1bit, �p���e�B����
	UART2_LCRH = 0x70;

	// CTS/RTS����, ��M/���M�L��, ���[�v����, UART�L��
	UART2_CR = 0x0301;

	return 0;
}

// 1�o�C�g���M
void uart2_putc(char byte) {
	while (UART2_FR & (1U << 5));
	UART2_DR = (uint32_t)byte;
}

// ������(�I�[NULL)���M
void uart2_puts(const char *str) {
	while (*str) {
		uart2_putc(*str++);
	}
}

// 1�o�C�g��M
char uart2_getc() {
	while (UART2_FR & (1U << 4));
	return (char)(UART2_DR & 0xFF);
}

