#ifndef RPI_H
#define RPI_H

#include <stdint.h>
#include <stdbool.h>

extern void *RPi_IO_Base_Addr;
extern void *RPi_LocalPeripheralsAddr;
extern uint32_t CoreExecute(uint32_t CoreID, void *Address);

#endif
