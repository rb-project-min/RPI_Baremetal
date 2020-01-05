#include <stdint.h>
#include <stdbool.h>

#include "rpi.h"
#include "LED.h"
#include "UART.h"

uint32_t main() {

	uart_init(0, 921600);
	uart_puts(0, "UART0�e�X�g\n");

	uart_init(1, 921600);
	uart_puts(1, "UART1�e�X�g\n");

	uart_init(2, 921600);
	uart_puts(2, "UART2�e�X�g\n");

	uart_init(3, 921600);
	uart_puts(3, "UART3�e�X�g\n");

	uart_init(4, 921600);
	uart_puts(4, "UART4�e�X�g\n");

	uart_init(5, 921600);
	uart_puts(5, "UART5�e�X�g\n");

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
