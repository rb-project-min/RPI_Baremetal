#include <stddef.h>

#include "uart1.h"
#include "GPIO.h"
#include "mailbox.h"

uint32_t uart1_init(uint32_t Baud) {

// Step1. UART�̎��O�ݒ�
	// UART1�̗L����
	AUX_ENABLES |= 0x00000001;
	// ���M ��M�̖�����
	UART1_CNTL = 0x00000000;

// Step 2: GPIO��UART��
	// GPIO �v���A�b�v�ݒ�
	GPIO_Pull(14, GPPUD_UP);
	GPIO_Pull(15, GPPUD_UP);

	// GPIO �t�@���N�V�����ݒ�
	GPIO_Mode(14, GPF_ALT5);
	GPIO_Mode(15, GPF_ALT5);

// Step 3: �{�[���[�g�ݒ�
	UART1_BAUD = (GetCoreCurrentClock() / (4 * Baud) - 1) >> 1;

// Step 4: ���M ��M�̗L����
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

	return 0;
}

// 1�o�C�g���M
void uart1_putc(char byte) {
	while (!(UART1_LSR & (1U << 6)) && !(UART1_LSR & (1U << 5)));
	UART1_IO = (uint32_t)byte;
}

// ������(�I�[NULL)���M
void uart1_puts(const char *str) {
	while (*str) {
		uart1_putc(*str++);
	}
}

// 1�o�C�g��M
char uart1_getc() {
	while (!(UART1_LSR & (1U << 0)));
	return UART1_IO;
}

