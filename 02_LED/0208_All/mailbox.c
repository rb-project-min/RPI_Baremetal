#include "mailbox.h"

// メールボックスメッセージバッファ
volatile uint32_t __attribute__((aligned(16))) mbox[256];

void gpio_set_config(uint32_t ExpGPIO, bool output, bool active_low, bool term_en, bool pull_up, bool initial_state)
{
	mbox[0] = 12 * 4;								// バッファサイズ(ヘッダーからパディングまで)
	mbox[1] = RPI_FIRMWARE_STATUS_REQUEST;			// リクエスト/レスポンス

	mbox[2] = RPI_FIRMWARE_SET_GPIO_CONFIG;			// タグ
	mbox[3] = 6 * 4;								// バッファサイズ(このタグの応答のみ)
	mbox[4] = RPI_FIRMWARE_STATUS_REQUEST;			// リクエスト/レスポンス
	mbox[5] = ExpGPIO + 128;						// ExpGPIO No.
	mbox[6] = output ? 1 : 0;						// ExpGPIOの入出力 0:入力 1:出力
	mbox[7] = active_low ? 1 : 0;					// ExpGPIOの反転 0:なし 1:あり
	mbox[8] = term_en ? 1 : 0;						// ExpGPIOのプルアップ/ダウンの有無 0:なし 1:あり
	mbox[9] = pull_up ? 1 : 0;						// ExpGPIOのプルアップ/ダウン 0:プルダウン 1:プルアップ
	mbox[10] = initial_state ? 1 : 0;				// ExpGPIOの初期値 0:LOW 1:HIGH (反転時 0:HIGH 1:LOW)
	mbox[11] = RPI_FIRMWARE_PROPERTY_END;

	mailbox(mbox, 8);
}

uint32_t gpio_get_config(uint32_t ExpGPIO, bool *output, bool *active_low, bool *term_en, bool *pull_up, bool *state)
{
	mbox[0] = 12 * 4;								// バッファサイズ(ヘッダーからパディングまで)
	mbox[1] = RPI_FIRMWARE_STATUS_REQUEST;			// リクエスト/レスポンス

	mbox[2] = RPI_FIRMWARE_GET_GPIO_CONFIG;			// タグ
	mbox[3] = 6 * 4;								// バッファサイズ(このタグの応答のみ)
	mbox[4] = RPI_FIRMWARE_STATUS_REQUEST;							// リクエスト/レスポンス
	mbox[5] = ExpGPIO + 128;						// ExpGPIO No.
	mbox[6] = 0;									// ExpGPIOの入出力 0:入力 1:出力
	mbox[7] = 0;									// ExpGPIOの反転 0:なし 1:あり
	mbox[8] = 0;									// ExpGPIOのプルアップ/ダウンの有無 0:なし 1:あり
	mbox[9] = 0;									// ExpGPIOのプルアップ/ダウン 0:プルダウン 1:プルアップ
	mbox[10] = 0;									// ExpGPIOの初期値 0:LOW 1:HIGH (反転時 0:HIGH 1:LOW)
	mbox[11] = RPI_FIRMWARE_PROPERTY_END;

	mailbox(mbox, 8);

	*output = mbox[6];
	*active_low = mbox[7];
	*term_en = mbox[8];
	*pull_up = mbox[9];
	*state = mbox[10];
	
	return ExpGPIO;
}

void mailbox_SetGpioState(uint32_t ExpGPIO, bool state)
{
	mbox[0] = 8 * 4;								// バッファサイズ(ヘッダーからパディングまで)
	mbox[1] = RPI_FIRMWARE_STATUS_REQUEST;			// リクエスト/レスポンス

	mbox[2] = RPI_FIRMWARE_SET_GPIO_STATE;			// タグ
	mbox[3] = 8;									// バッファサイズ(このタグの応答のみ)
	mbox[4] = RPI_FIRMWARE_STATUS_REQUEST;							// リクエスト/レスポンス
	mbox[5] = ExpGPIO + 128;						// ExpGPIO No.
	mbox[6] = state ? 1 : 0;						// ExpGPIOの状態 0:OFF 1:ON
	mbox[7] = RPI_FIRMWARE_PROPERTY_END;

	mailbox(mbox, 8);
}

bool mailbox_GetGpioState(uint32_t ExpGPIO)
{
	mbox[0] = 8 * 4;								// バッファサイズ(ヘッダーからパディングまで)
	mbox[1] = RPI_FIRMWARE_STATUS_REQUEST;			// リクエスト/レスポンス

	mbox[2] = RPI_FIRMWARE_GET_GPIO_STATE;			// タグ
	mbox[3] = 8;									// バッファサイズ(このタグの応答のみ)
	mbox[4] = RPI_FIRMWARE_STATUS_REQUEST;							// リクエスト/レスポンス
	mbox[5] = ExpGPIO + 128;						// ExpGPIO No.
	mbox[6] = 0;									// ExpGPIOの状態 0:OFF 1:ON
	mbox[7] = RPI_FIRMWARE_PROPERTY_END;

	mailbox(mbox, 8);

	return mbox[6];
}

extern uint32_t BoardRevision;

uint32_t mailbox_GetBoardRevision()
{
	return BoardRevision;
}
