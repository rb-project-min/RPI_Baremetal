#ifndef MAILBOX_H
#define MAILBOX_H

#include <stdint.h>
#include <stdbool.h>

extern void mailbox(volatile uint32_t *AlinedBuffer, uint32_t Channel);

// メールボックスメッセージバッファ
extern volatile uint32_t __attribute__((aligned(16))) mbox[256];

#define RPI_FIRMWARE_PROPERTY_END			0x00000000

#define RPI_FIRMWARE_STATUS_REQUEST			0x00000000
#define RPI_FIRMWARE_STATUS_SUCCESS			0x80000000
#define RPI_FIRMWARE_STATUS_ERROR			0x80000001

#define RPI_FIRMWARE_GET_BOARD_REVISION		0x00010002
#define RPI_FIRMWARE_GET_VC_MEMORY			0x00010006
#define RPI_FIRMWARE_GET_GPIO_STATE			0x00030041
#define RPI_FIRMWARE_SET_GPIO_STATE			0x00038041
#define RPI_FIRMWARE_GET_GPIO_CONFIG		0x00030043
#define RPI_FIRMWARE_SET_GPIO_CONFIG		0x00038043

void gpio_set_config(uint32_t ExpGPIO, bool output, bool active_low, bool term_en, bool pull_up, bool initial_state);
uint32_t gpio_get_config(uint32_t ExpGPIO, bool *output, bool *active_low, bool *term_en, bool *pull_up, bool *state);
void mailbox_SetGpioState(uint32_t ExpGPIO, bool state);
bool mailbox_GetGpioState(uint32_t ExpGPIO);
uint32_t mailbox_GetBoardRevision();

#endif
