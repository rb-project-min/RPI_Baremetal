#include <stdint.h>
#include <stdbool.h>

extern void *RPi_IO_Base_Addr;
extern uint32_t CoreExecute(uint32_t CoreID, void *Address);
extern void mailbox(volatile uint32_t *AlinedBuffer, uint32_t Channel);

#define GPIO_BASE     (RPi_IO_Base_Addr + 0x00200000)

/* GPIO Function Select */
#define GPFSEL0 (*(volatile uint32_t*)(GPIO_BASE + 0x00))
#define GPFSEL1 (*(volatile uint32_t*)(GPIO_BASE + 0x04))
#define GPFSEL2 (*(volatile uint32_t*)(GPIO_BASE + 0x08))
#define GPFSEL3 (*(volatile uint32_t*)(GPIO_BASE + 0x0C))
#define GPFSEL4 (*(volatile uint32_t*)(GPIO_BASE + 0x10))
#define GPFSEL5 (*(volatile uint32_t*)(GPIO_BASE + 0x14))

/* GPIO Pin Output Set */
#define GPSET0  (*(volatile uint32_t*)(GPIO_BASE + 0x1C))
#define GPSET1  (*(volatile uint32_t*)(GPIO_BASE + 0x20))

/* GPIO Pin Output Clear */
#define GPCLR0  (*(volatile uint32_t*)(GPIO_BASE + 0x28))
#define GPCLR1  (*(volatile uint32_t*)(GPIO_BASE + 0x2C))

// メールボックスメッセージバッファ
volatile uint32_t __attribute__((aligned(16))) mbox[8];

uint32_t main() {

	// GPIO42 出力に設定
	GPFSEL4 = (GPFSEL2 & ~(7 << (2 * 3))) | (1 << (2 * 3));

	// セットして待つ、クリアして待つ、を繰り返す。
	while (1) {
		GPSET1 = 1 << 10;
		
		mbox[0] = 8 * 4;								// バッファサイズ(ヘッダーからパディングまで)
		mbox[1] = 0;									// リクエスト/レスポンス
		mbox[2] = 0x00038041;							// タグ
		mbox[3] = 8;									// バッファサイズ(このタグの応答のみ)
		mbox[4] = 0;									// リクエスト/レスポンス
		mbox[5] = 2 + 128;								// ExpGPIO No.
		mbox[6] = 1;									// ExpGPIOの状態 0:OFF 1:ON
		mbox[7] = 0;
		mailbox(mbox, 8);

		for(volatile uint32_t i = 0; i < 3000000; i++);

		GPCLR1 = 1 << 10;

		mbox[0] = 8 * 4;								// バッファサイズ(ヘッダーからパディングまで)
		mbox[1] = 0;									// リクエスト/レスポンス
		mbox[2] = 0x00038041;							// タグ
		mbox[3] = 8;									// バッファサイズ(このタグの応答のみ)
		mbox[4] = 0;									// リクエスト/レスポンス
		mbox[5] = 2 + 128;								// ExpGPIO No.
		mbox[6] = 0;									// ExpGPIOの状態 0:OFF 1:ON
		mbox[7] = 0;
		mailbox(mbox, 8);

		for(volatile uint32_t i = 0; i < 3000000; i++);
	}

	return 0;
}
