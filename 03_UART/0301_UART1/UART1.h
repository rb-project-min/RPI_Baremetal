#ifndef UART1_H
#define UART1_H
#include <stdint.h>
#include <stdbool.h>

#include "rpi.h"

#ifndef AUX_BASE
#define AUX_BASE     (RPi_IO_Base_Addr + 0x00215000)

/* Auxiliary Interrupt status  */
#define AUX_IRQ            (*(volatile uint32_t*)(AUX_BASE + 0x00))

/* Auxiliary enables */
#define AUX_ENABLES        (*(volatile uint32_t*)(AUX_BASE + 0x04))
#endif

/* Mini Uart I/O Data */
// bit31- 8:		�\�� 0�Œ�
// bit 7- 0: RXBUF	��M�f�[�^�o�b�t�@(�Ǎ���)
// bit 7- 0: TXBUF	���M�f�[�^�o�b�t�@(������)
// bit 7- 0: DLL	Bandrate�ݒ�l ����8bit(DLAB=1��)
#define UART1_IO			(*(volatile uint32_t*)(AUX_BASE + 0x40))

/* Mini Uart Interrupt Enable */
// bit31- 4:		�\�� 0�Œ�
// bit 3: EDSSI		���� 0�Œ�
// bit 2: ELSI		���� 0�Œ�
// bit 1: ETBEI		���M���荞�݂̋���(0:�֎~ 1:����)
// bit 0: ERBFI		��M���荞�݂̋���(0:�֎~ 1:����)
// bit 7- 0: DLM	Bandrate�ݒ�l ���8bit(DLAB=1��)
#define UART1_IER		(*(volatile uint32_t*)(AUX_BASE + 0x44))

/* Mini Uart Interrupt Identify */
// bit31- 8:		�\�� 0�Œ�
// bit 7: FRMERR	����(�Ǎ���) 0�Œ�
// bit 6: TEMT		����(�Ǎ���) 0�Œ�
// bit 5- 4:		�\�� 0�Œ�
// bit 3: IRTO		����(�Ǎ���) 0�Œ�
// bit 2-1: IRID	���荞��ID(�Ǎ���)(0:���f����� 1:���M�o�b�t�@ 2:��M�\ 3:��M�X�e�[�^�X)
// bit 0: IRP		���荞�ݏ��(�Ǎ���)(0:�����݂��� 1:�����݂Ȃ�)
//
// bit 7- 6: RCVRT	FIFO�g���K���x��(������)(0:1Byte 1:4Byte 2:8Byte 3:14Byte)
// bit 5- 4:		�\�� 0�Œ�
// bit 3: DMAS		DMA���[�h�I��(������)(0:�ʏ� 1:DMA)
// bit 2: XMITFR	���M�o�b�t�@�N���A(������)(0:�N���A���Ȃ� 1:�N���A����)
// bit 1: RCVRFR	��M�o�b�t�@�N���A(������)(0:�N���A���Ȃ� 1:�N���A����)
// bit 0: FIFOEN	FIFO�C�l�[�u��(������)(0:���� 1:�L��)
#define UART1_IIR		(*(volatile uint32_t*)(AUX_BASE + 0x48))

/* Mini Uart Line Control */
#define UART1_LCR		(*(volatile uint32_t*)(AUX_BASE + 0x4C))
/* Mini Uart Modem Control */
#define UART1_MCR		(*(volatile uint32_t*)(AUX_BASE + 0x50))
/* Mini Uart Line Status */
#define UART1_LSR		(*(volatile uint32_t*)(AUX_BASE + 0x54))
/* Mini Uart Modem Status */
#define UART1_MSR		(*(volatile uint32_t*)(AUX_BASE + 0x58))
/* Mini Uart Scratch */
#define UART1_SCRATCH	(*(volatile uint32_t*)(AUX_BASE + 0x5C))
/* Mini Uart Extra Control */
#define UART1_CNTL		(*(volatile uint32_t*)(AUX_BASE + 0x60))
/* Mini Uart Extra Status */
#define UART1_STAT		(*(volatile uint32_t*)(AUX_BASE + 0x64))
/* Mini Uart Baudrate */
#define UART1_BAUD		(*(volatile uint32_t*)(AUX_BASE + 0x68))

uint32_t uart1_init(uint32_t Baud);

void uart1_putc(char byte);
void uart1_puts(const char *str);
char uart1_getc();

#endif
