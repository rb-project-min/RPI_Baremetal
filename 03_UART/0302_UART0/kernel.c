#include <stdint.h>
#include <stdbool.h>

#include "rpi.h"
#include "LED.h"
#include "UART0.h"

uint32_t main() {

	uart0_init(921600);

	uart0_puts("UART0テスト\n");

	Act_LED_Init();
	Power_LED_Init();

	// セットして待つ、クリアして待つ、を繰り返す。
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
