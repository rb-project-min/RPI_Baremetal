#include <stdint.h>
#include <stdbool.h>

#include "rpi.h"
#include "LED.h"
#include "UART1.h"

uint32_t main() {

	uart1_init(921600);

	uart1_puts("UART1�e�X�g\n");

	Act_LED_Init();
	Power_LED_Init();

	// �Z�b�g���đ҂A�N���A���đ҂A���J��Ԃ��B
	while (1) {
		Act_LED_ON();
		Power_LED_OFF();

		for(volatile uint32_t i = 0; i < 3000000; i++);

		Act_LED_OFF();
		Power_LED_ON();

		for(volatile uint32_t i = 0; i < 3000000; i++);
	}

	return 0;
}
