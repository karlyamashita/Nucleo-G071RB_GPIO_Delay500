/*
 * PollingRoutine.h
 *
 *  Created on: Oct 24, 2023
 *      Author: karl.yamashita
 *
 *
 *      Template
 */

#ifndef INC_POLLINGROUTINE_H_
#define INC_POLLINGROUTINE_H_


/*

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <ctype.h>
#define Nop() asm(" NOP ")

#include "PollingRoutine.h"

*/
#ifndef __weak
#define __weak __attribute__((weak))
#endif


typedef union
{
	struct
	{
		uint8_t data[2]; // byte 0 is current state, byte 1 is last state
	}Byte;
	struct
	{
		uint8_t pa1:1;
		uint8_t pa2:1;
		uint8_t pc13:1;
		uint8_t :5;
	}Status;
}InputStatus_t;


void PollingInit(void);
void PollingRoutine(void);

void GPIO_Check(InputStatus_t *input);
void GPIO_UpdatePinStatus(InputStatus_t *input, uint8_t pin, GPIO_PinState pinStatus);

void PA10_On(void);
void PA10_Off(void);
void PB12_On(void);
void PB12_Off(void);

void GPIO_Callback(uint16_t GPIO_Pin);

#endif /* INC_POLLINGROUTINE_H_ */
