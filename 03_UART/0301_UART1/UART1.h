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
// bit31- 8:		予約 0固定
// bit 7- 0: RXBUF	受信データバッファ(読込時)
// bit 7- 0: TXBUF	送信データバッファ(書込時)
// bit 7- 0: DLL	Bandrate設定値 下位8bit(DLAB=1時)
#define UART1_IO			(*(volatile uint32_t*)(AUX_BASE + 0x40))

/* Mini Uart Interrupt Enable */
// bit31- 4:		予約 0固定
// bit 3: EDSSI		無効 0固定
// bit 2: ELSI		無効 0固定
// bit 1: ETBEI		送信割り込みの許可(0:禁止 1:許可)
// bit 0: ERBFI		受信割り込みの許可(0:禁止 1:許可)
// bit 7- 0: DLM	Bandrate設定値 上位8bit(DLAB=1時)
#define UART1_IER		(*(volatile uint32_t*)(AUX_BASE + 0x44))

/* Mini Uart Interrupt Identify */
// bit31- 8:		予約 0固定
// bit 7: FRMERR	無効(読込時) 0固定
// bit 6: TEMT		無効(読込時) 0固定
// bit 5- 4:		予約 0固定
// bit 3: IRTO		無効(読込時) 0固定
// bit 2-1: IRID	割り込みID(読込時)(0:モデム状態 1:送信バッファ 2:受信可能 3:受信ステータス)
// bit 0: IRP		割り込み状態(読込時)(0:割込みあり 1:割込みなし)
//
// bit 7- 6: RCVRT	FIFOトリガレベル(書込時)(0:1Byte 1:4Byte 2:8Byte 3:14Byte)
// bit 5- 4:		予約 0固定
// bit 3: DMAS		DMAモード選択(書込時)(0:通常 1:DMA)
// bit 2: XMITFR	送信バッファクリア(書込時)(0:クリアしない 1:クリアする)
// bit 1: RCVRFR	受信バッファクリア(書込時)(0:クリアしない 1:クリアする)
// bit 0: FIFOEN	FIFOイネーブル(書込時)(0:無効 1:有効)
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
