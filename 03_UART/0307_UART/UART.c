#include <stddef.h>

#include "UART.h"
#include "GPIO.h"
#include "mailbox.h"

uint32_t uart_init(uint32_t id, uint32_t Baud) {
	if(id > 5) return 1;
	if(id > 1 && (uint32_t)RPi_IO_Base_Addr < 0x80000000) return 1;

	void *UART_BASE = RPi_IO_Base_Addr + 0x00201000 + id * 0x200;

// Step1. UART�̎��O�ݒ�
	if(id == 1) {
		// UART1�̗L����
		AUX_ENABLES |= 0x00000001;
		// ���M ��M�̖�����
		UART1_CNTL = 0x00000000;
	}
	else {
		// UART�̖�����
		UART_CR = 0;
	}

// Step 2: GPIO��UART��
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
// Step 3: �{�[���[�g�ݒ�
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

// Step 4: ���M ��M�̗L����
	if(id == 1) {
		// ���荞�݂��N���A����
		UART1_IER = 0x00000000;

		// 8bit�ʐM��L���ɂ���D
		UART1_LCR = 0x00000003;

		// �S�Ă̊��荞�݂�L���� 
		UART1_IER = 0x00000003;

		// FIFO���N���A
		UART1_IIR = 0x000000C7;

		// ���M ��M�̗L����
		UART1_CNTL = 0x00000003;
	}
	else {
		UART_ICR  = 0x7FF;     // ���荞�݂��N���A����

		// �f�[�^��8bit, FIFO�L��, �X�g�b�v�r�b�g 1bit, �p���e�B����
		UART_LCRH = 0x70;

		// CTS/RTS����, ��M/���M�L��, ���[�v����, UART�L��
		UART_CR = 0x0301;
	}
	return 0;
}

// 1�o�C�g���M
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

// ������(�I�[NULL)���M
void uart_puts(uint32_t id, const char *str) {
	if(id > 5) return;
	if(id > 1 && (uint32_t)RPi_IO_Base_Addr < 0x80000000) return;

	while (*str) {
		uart_putc(id, *str++);
	}
}

// 1�o�C�g��M
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

