#include <stdint.h>
#include <stdbool.h>

#include "GPIO.h"
#include "mailbox.h"

#include "LED.h"

uint32_t Act_LED_Mode = 0x00;
uint32_t Pwr_LED_Mode = 0x00;

void Act_LED_Init()
{
	uint32_t Rev = mailbox_GetBoardRevision();

	if(Rev < 0x10) Act_LED_Mode = 0x01;				// RaspberryPi Model A/Model B
	else if(Rev < 0x16) Act_LED_Mode = 0x02;		// RaspberryPi Model A+/Model B+ ‚Ü‚½‚Í Compute Module
	else {
		Rev &= 0xFF0;
		if(Rev == 0x000) Act_LED_Mode = 0x01;		// RaspberryPi Model A
		else if(Rev == 0x010) Act_LED_Mode = 0x01;	// RaspberryPi Model B
		else if(Rev == 0x020) Act_LED_Mode = 0x02;	// RaspberryPi Model A+
		else if(Rev == 0x030) Act_LED_Mode = 0x02;	// RaspberryPi Model B+
		else if(Rev == 0x040) Act_LED_Mode = 0x02;	// RaspberryPi2 Model B
		else if(Rev == 0x060) Act_LED_Mode = 0x02;	// Compute Module
		else if(Rev == 0x080) Act_LED_Mode = 0x03;	// RaspberryPi3 Model B
		else if(Rev == 0x090) Act_LED_Mode = 0x02;	// RaspberryPi Zero
		else if(Rev == 0x0A0) Act_LED_Mode = 0x03;	// Compute Module 3
		else if(Rev == 0x0C0) Act_LED_Mode = 0x02;	// RaspberryPi Zero W
		else if(Rev == 0x0D0) Act_LED_Mode = 0x04;	// RaspberryPi3 Model B+
		else if(Rev == 0x0E0) Act_LED_Mode = 0x04;	// RaspberryPi3 Model A+
		else if(Rev == 0x100) Act_LED_Mode = 0x03;	// Compute Module 3+
		else if(Rev == 0x110) Act_LED_Mode = 0x05;	// RaspberryPi4 Model B
		else Act_LED_Mode = 0x05;					// ¡ŒãV‚½‚É”­”„‚³‚ê‚éê‡,4B‚Æ“¯‚¶‚Æ‰¼’è‚·‚é(—vC³)
	}

	if(Act_LED_Mode == 0x01) GPIO_Mode(16, GPF_OUTPUT);
	else if(Act_LED_Mode == 0x02) GPIO_Mode(47, GPF_OUTPUT);
	else if(Act_LED_Mode == 0x04) GPIO_Mode(29, GPF_OUTPUT);
	else if(Act_LED_Mode == 0x05) GPIO_Mode(42, GPF_OUTPUT);
}

void Act_LED_ON()
{
	if(Act_LED_Mode == 0x01) GPIO_Write(16, true);
	else if(Act_LED_Mode == 0x02) GPIO_Write(47, true);
	else if(Act_LED_Mode == 0x03) mailbox_SetGpioState(2, true);
	else if(Act_LED_Mode == 0x04) GPIO_Write(29, true);
	else if(Act_LED_Mode == 0x05) GPIO_Write(42, true);
}

void Act_LED_OFF()
{
	if(Act_LED_Mode == 0x01) GPIO_Write(16, false);
	else if(Act_LED_Mode == 0x02) GPIO_Write(47, false);
	else if(Act_LED_Mode == 0x03) mailbox_SetGpioState(2, false);
	else if(Act_LED_Mode == 0x04) GPIO_Write(29, false);
	else if(Act_LED_Mode == 0x05) GPIO_Write(42, false);
}

bool Act_LED_State()
{
	if(Act_LED_Mode == 0x01) return GPIO_Read(16);
	else if(Act_LED_Mode == 0x02) return GPIO_Read(47);
	else if(Act_LED_Mode == 0x03) return mailbox_GetGpioState(2);
	else if(Act_LED_Mode == 0x04) return GPIO_Read(29);
	else if(Act_LED_Mode == 0x05) return GPIO_Read(42);
	else return false;
}

void Power_LED_Init()
{
	uint32_t Rev = mailbox_GetBoardRevision();

	if(Rev < 0x10) Pwr_LED_Mode = 0x01;				// RaspberryPi Model A/Model B
	else if(Rev == 0x11) Pwr_LED_Mode = 0x01;		// Compute Module
	else if(Rev == 0x14) Pwr_LED_Mode = 0x01;		// Compute Module
	else if(Rev < 0x16) Pwr_LED_Mode = 0x02;		// RaspberryPi Model A+/Model B+
	else {
		Rev &= 0xFF0;
		if(Rev == 0x000) Pwr_LED_Mode = 0x01;		// RaspberryPi Model A
		else if(Rev == 0x010) Pwr_LED_Mode = 0x01;	// RaspberryPi Model B
		else if(Rev == 0x020) Pwr_LED_Mode = 0x02;	// RaspberryPi Model A+
		else if(Rev == 0x030) Pwr_LED_Mode = 0x02;	// RaspberryPi Model B+
		else if(Rev == 0x040) Pwr_LED_Mode = 0x02;	// RaspberryPi2 Model B
		else if(Rev == 0x060) Pwr_LED_Mode = 0x01;	// Compute Module
		else if(Rev == 0x080) Pwr_LED_Mode = 0x03;	// RaspberryPi3 Model B
		else if(Rev == 0x090) Pwr_LED_Mode = 0x01;	// RaspberryPi Zero
		else if(Rev == 0x0A0) Pwr_LED_Mode = 0x01;	// Compute Module 3
		else if(Rev == 0x0C0) Pwr_LED_Mode = 0x01;	// RaspberryPi Zero W
		else if(Rev == 0x0D0) Pwr_LED_Mode = 0x04;	// RaspberryPi3 Model B+
		else if(Rev == 0x0E0) Pwr_LED_Mode = 0x04;	// RaspberryPi3 Model A+
		else if(Rev == 0x100) Pwr_LED_Mode = 0x01;	// Compute Module 3+
		else if(Rev == 0x110) Pwr_LED_Mode = 0x04;	// RaspberryPi4 Model B
		else Pwr_LED_Mode = 0x04;					// ¡ŒãV‚½‚É”­”„‚³‚ê‚éê‡,4B‚Æ“¯‚¶‚Æ‰¼’è‚·‚é(—vC³)
	}

	if(Pwr_LED_Mode == 0x02) GPIO_Mode(35, GPF_OUTPUT);
	else if(Pwr_LED_Mode == 0x03) gpio_set_config(7, true, false, false, false, false);
}

void Power_LED_ON()
{
	if(Pwr_LED_Mode == 0x02) GPIO_Write(35, true);
	else if(Pwr_LED_Mode == 0x03) mailbox_SetGpioState(7, true);
	else if(Pwr_LED_Mode == 0x04) mailbox_SetGpioState(2, false);
}

void Power_LED_OFF()
{
	if(Pwr_LED_Mode == 0x02) GPIO_Write(35, false);
	else if(Pwr_LED_Mode == 0x03) mailbox_SetGpioState(7, false);
	else if(Pwr_LED_Mode == 0x04) mailbox_SetGpioState(2, true);
}

bool Power_LED_State()
{
	if(Act_LED_Mode == 0x02) return GPIO_Read(35);
	else if(Act_LED_Mode == 0x03) return mailbox_GetGpioState(7);
	else if(Act_LED_Mode == 0x04) return mailbox_GetGpioState(2);
	else return false;
}
