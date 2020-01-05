#ifndef UART0_H
#define UART0_H
#include <stdint.h>
#include <stdbool.h>

#include "rpi.h"

#define UART0_BASE		(RPi_IO_Base_Addr + 0x00201000)

/* �f�[�^���W�X�^ */
// bit31-12:			�\�� 0�Œ�
// bit11:	 OE			�I�[�o�[�����G���[
// bit10:	 BE			�u���[�N�G���[
// bit 9:	 PE			�p���e�B�G���[
// bit 8:	 FE			�t���[�~���O�G���[
// bit 7- 0: DATA		RX/TX�f�[�^
#define UART0_DR		(*(volatile uint32_t*)(UART0_BASE + 0x00))

/* RSRECR Register */
// bit31- 4:			�\�� 0�Œ�
// bit 3:	 OE			�I�[�o�[�����G���[
// bit 2:	 BE			�u���[�N�G���[
// bit 1:	 PE			�p���e�B�G���[
// bit 0:	 FE			�t���[�~���O�G���[
#define UART0_RSRECR	(*(volatile uint32_t*)(UART0_BASE + 0x04))

/* �t���O���W�X�^ */
// bit31- 9:			�\�� 0�Œ�
// bit 8:	 RI			�����N�C���W�P�[�^(���T�|�[�g)
// bit 7:	 TXFE		���MFIFO �G���v�e�B
// bit 6:	 RXFF		��MFIFO �t��
// bit 5:	 TXFF		���MFIFO �t��
// bit 4:	 RXFE		��MFIFO �G���v�e�B
// bit 3:	 BUSY		UART �r�W�[
// bit 2:	 DCD		�f�[�^�L�����A���o(���T�|�[�g)
// bit 1:	 DSR		�f�[�^�Z�b�g��������(���T�|�[�g)
// bit 0:	 CTS		���M�N���A
#define UART0_FR		(*(volatile uint32_t*)(UART0_BASE + 0x18))

/* �����{�[���[�g�������W�X�^ */
// bit31-16:			�\�� 0�Œ�
// bit15- 0: IBRD		�o���h���[�g�f�B�o�C�_�[(��������)
//						IBRD = 3,000,000/(16*Baudrate)
#define UART0_IBRD		(*(volatile uint32_t*)(UART0_BASE + 0x24))

/* �����{�[���[�g�������W�X�^ */
// bit31- 6:			�\�� 0�Œ�
// bit 5- 0: FBRD		�o���h���[�g�f�B�o�C�_�[(��������)
//						FBRD = (24,000,000/Baudrate-IBRD*128+1)/2
#define UART0_FBRD		(*(volatile uint32_t*)(UART0_BASE + 0x28))

/* ���C�����䃌�W�X�^ */
// bit31- 8:			�\�� 0�Œ�
// bit 7:	 SPS		�X�e�B�b�N�p���e�B 0:���� 1:�L��
// bit 6- 5: WLEN		�f�[�^�� 0:5bit 1:6bit 2:7bit 3:8bit
// bit 4:	 FEN		FIFO�L���� 0:���� 1:�L��
// bit 3:	 STP2		�X�g�b�v�r�b�g 0:1bit 1:2bit
// bit 2:	 EPS		�p���e�B�r�b�g 0:� 1:����
// bit 1:	 PEN		�p���e�B�L���� 0:���� 1:�L��
// bit 0:	 BRK		�u���[�N���M 0:���Ȃ� 1:����
#define UART0_LCRH		(*(volatile uint32_t*)(UART0_BASE + 0x2C))

/* �R���g���[�����W�X�^ */
// bit31-16:			�\�� 0�Œ�
// bit15:	 CTSEN		CTS�n�[�h�E�F�A�t���[���� 0:���� 1:�L��
// bit14:	 RTSEN		RTS�n�[�h�E�F�A�t���[���� 0:���� 1:�L��
// bit13:	 OUT2		���T�|�[�g
// bit12:	 OUT1		���T�|�[�g
// bit11:	 RTS		���M�v���M�� 0:HIGH 1:LOW
// bit10:	 DTR		���T�|�[�g
// bit 9:	 RXE		��M�L���� 0:���� 1:�L��
// bit 8:	 TXE		���M�L���� 0:���� 1:�L��
// bit 7:	 LBE		���[�v�o�b�N 0:���� 1:�L��
// bit 6- 3: 			�\�� 0�Œ�
// bit 2:	 SIRLP		���T�|�[�g
// bit 1:	 SIREN		���T�|�[�g
// bit 0:	 UARTEN		UART�L���� 0:���� 1:�L��
#define UART0_CR		(*(volatile uint32_t*)(UART0_BASE + 0x30))

/* FIFO���荞�݃��x�����W�X�^ */
// bit31-12:			�\�� 0�Œ�
// bit11- 9: RXIFPSEL	���T�|�[�g
// bit 8- 6: TXIFPSEL	���T�|�[�g
// bit 5- 3: RXIFLSEL	��MFIFO���荞�݃��x�� 0:1/8 1:1/4 2:1/2 3:3/4 4:7/8
// bit 2- 0: TXIFLSEL	���MFIFO���荞�݃��x�� 0:1/8 1:1/4 2:1/2 3:3/4 4:7/8
#define UART0_IFLS		(*(volatile uint32_t*)(UART0_BASE + 0x34))

/* ���荞�݃}�X�N���W�X�^ */
// bit31-11:			�\�� 0�Œ�
// bit10:	 OEIM		�I�[�o�[�����G���[���荞�݃}�X�N 0:�N���A 1:�}�X�N
// bit 9:	 BEIM		�u���[�N�G���[���荞�݃}�X�N 0:�N���A 1:�}�X�N
// bit 8:	 PEIM		�p���e�B�G���[���荞�݃}�X�N 0:�N���A 1:�}�X�N
// bit 7:	 FEIM		�t���[�~���O�G���[���荞�݃}�X�N 0:�N���A 1:�}�X�N
// bit 6:	 RTIM		��M�^�C���A�E�g���荞�݃}�X�N 0:�N���A 1:�}�X�N
// bit 5:	 TXIM		���M���荞�݃}�X�N 0:�N���A 1:�}�X�N
// bit 4:	 RXIM		��M���荞�݃}�X�N 0:�N���A 1:�}�X�N
// bit 3:	 DSRMIM		���T�|�[�g
// bit 2:	 DCDMIM		���T�|�[�g
// bit 1:	 CTSMIM		nUARTCTS���f�����荞�݃}�X�N 0:�N���A 1:�}�X�N
// bit 0:	 RIMIM		���T�|�[�g
#define UART0_IMSC		(*(volatile uint32_t*)(UART0_BASE + 0x28))

/* ���荞�݃��W�X�^(���f�[�^) */
// bit31-11:			�\�� 0�Œ�
// bit10:	 OERIS		�I�[�o�[�����G���[���荞�� 0:�Ȃ� 1:����
// bit 9:	 BERIS		�u���[�N�G���[���荞�� 0:�Ȃ� 1:����
// bit 8:	 PERIS		�p���e�B�G���[���荞�� 0:�Ȃ� 1:����
// bit 7:	 FERIS		�t���[�~���O�G���[���荞�� 0:�Ȃ� 1:����
// bit 6:	 RTRIS		��M�^�C���A�E�g���荞�� 0:�Ȃ� 1:����
// bit 5:	 TXRIS		���M���荞�� 0:�Ȃ� 1:����
// bit 4:	 RXRIS		��M���荞�� 0:�Ȃ� 1:����
// bit 3:	 DSRMRIS	���T�|�[�g
// bit 2:	 DCDMRIS	���T�|�[�g
// bit 1:	 CTSMRIS	nUARTCTS���f�����荞�� 0:�Ȃ� 1:����
// bit 0:	 RIMRIS		���T�|�[�g
#define UART0_RIS		(*(volatile uint32_t*)(UART0_BASE + 0x3C))

/* ���荞�݃��W�X�^(�}�X�N) */
// bit31-11:			�\�� 0�Œ�
// bit10:	 OEMIS		�I�[�o�[�����G���[���荞�� 0:�Ȃ� 1:����
// bit 9:	 BEMIS		�u���[�N�G���[���荞�� 0:�Ȃ� 1:����
// bit 8:	 PEMIS		�p���e�B�G���[���荞�� 0:�Ȃ� 1:����
// bit 7:	 FEMIS		�t���[�~���O�G���[���荞�� 0:�Ȃ� 1:����
// bit 6:	 RTMIS		��M�^�C���A�E�g���荞�� 0:�Ȃ� 1:����
// bit 5:	 TXMIS		���M���荞�� 0:�Ȃ� 1:����
// bit 4:	 RXMIS		��M���荞�� 0:�Ȃ� 1:����
// bit 3:	 DSRMMIS	���T�|�[�g
// bit 2:	 DCDMMIS	���T�|�[�g
// bit 1:	 CTSMMIS	nUARTCTS���f�����荞�� 0:�Ȃ� 1:����
// bit 0:	 RIMMIS		���T�|�[�g
#define UART0_MIS		(*(volatile uint32_t*)(UART0_BASE + 0x40))

/* ���荞�݃N���A���W�X�^ */
// bit31-11:			�\�� 0�Œ�
// bit10:	 OEIC		�I�[�o�[�����G���[���荞�݃N���A 0:���Ȃ� 1:����
// bit 9:	 BEIC		�u���[�N�G���[���荞�݃N���A 0:���Ȃ� 1:����
// bit 8:	 PEIC		�p���e�B�G���[���荞�݃N���A 0:���Ȃ� 1:����
// bit 7:	 FEIC		�t���[�~���O�G���[���荞�݃N���A 0:���Ȃ� 1:����
// bit 6:	 RTIC		��M�^�C���A�E�g���荞�݃N���A 0:���Ȃ� 1:����
// bit 5:	 TXIC		���M���荞�݃N���A 0:���Ȃ� 1:����
// bit 4:	 RXIC		��M���荞�݃N���A 0:���Ȃ� 1:����
// bit 3:	 DSRMIC		���T�|�[�g
// bit 2:	 DCDMIC		���T�|�[�g
// bit 1:	 CTSMIC		nUARTCTS���f�����荞�݃N���A 0:���Ȃ� 1:����
// bit 0:	 RIMIC		���T�|�[�g
#define UART0_ICR		(*(volatile uint32_t*)(UART0_BASE + 0x44))

#define UART0_ITCR		(*(volatile uint32_t*)(UART0_BASE + 0x80))
#define UART0_ITIP		(*(volatile uint32_t*)(UART0_BASE + 0x84))
#define UART0_ITOP		(*(volatile uint32_t*)(UART0_BASE + 0x88))
#define UART0_TDR		(*(volatile uint32_t*)(UART0_BASE + 0x8C))

uint32_t uart0_init(uint32_t Baud);

void uart0_putc(char byte);
void uart0_puts(const char *str);
char uart0_getc();

#endif
