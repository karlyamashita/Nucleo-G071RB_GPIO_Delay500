/*
 * PollingRoutine.c
 *
 *  Created on: Oct 24, 2023
 *      Author: karl.yamashita
 *
 *
 *      Template for projects.
 *
 *      The object of this PollingRoutine.c/h files is to not have to write code in main.c which already has a lot of generated code.
 *      It is cumbersome having to scroll through all the generated code for your own code and having to find a USER CODE section so your code is not erased when CubeMX re-generates code.
 *      
 *      Direction: Call PollingInit before the main while loop. Call PollingRoutine from within the main while loop
 * 
 *      Example;
        // USER CODE BEGIN WHILE
        PollingInit();
        while (1)
        {
            PollingRoutine();
            // USER CODE END WHILE

            // USER CODE BEGIN 3
        }
        // USER CODE END 3

 */


#include "main.h"


InputStatus_t inputs = {0};


void PollingInit(void)
{
	PB12_On(); // init to 1
	PA10_Off(); // init to 0

	TimerCallbackRegisterOnly(&timerCallback, PA10_Off);
}

void PollingRoutine(void)
{
	TimerCallbackCheck(&timerCallback);

	GPIO_Check(&inputs);
}

void GPIO_Check(InputStatus_t *input)
{
	if(input->Byte.data[1] != input->Byte.data[0]) // check for change, else do nothing.
	{
		input->Byte.data[1] = input->Byte.data[0]; // copy current state to last state

		switch(input->Byte.data[0]) // index of truth table
		{
		case 0:
			PB12_Off(); // pin is low
			PA10_On(); // pin is high
			TimerCallbackTimerStart(&timerCallback, PA10_Off, 500, TIMER_NO_REPEAT); // start timer to turn off PA10
			break;
		case 1: // all other case, PA12 is On (high state)
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		default:
			PB12_On(); // pin is high
			break;
		}
	}
}

void PA10_On(void)
{
	HAL_GPIO_WritePin(PA10_GPIO_Port, PA10_Pin, GPIO_PIN_SET);
}

void PA10_Off(void)
{
	HAL_GPIO_WritePin(PA10_GPIO_Port, PA10_Pin, GPIO_PIN_RESET);
}

void PB12_On(void)
{
	HAL_GPIO_WritePin(PB12_GPIO_Port, PB12_Pin, GPIO_PIN_SET);
}

void PB12_Off(void)
{
	HAL_GPIO_WritePin(PB12_GPIO_Port, PB12_Pin, GPIO_PIN_RESET);
}

/*
 * Description: Update pin status for specific input.
 * Input: pin data structure, pin to update, the pin state
 */
void GPIO_UpdatePinStatus(InputStatus_t *input, uint8_t pin, GPIO_PinState pinStatus)
{
	switch(pin)
	{
	case 0:
		input->Status.pa1 = pinStatus;
		break;
	case 1:
		input->Status.pa2 = pinStatus;
		break;
	case 2:
		input->Status.pc13 = pinStatus;
		break;
	default:
		// invalid pin
		break;
	}
}

/*
 * Description: Functions as a HAL GPIO callback.
 * Note: no sw debounce incorporated.
 * See YouTube video to learn how to debounce push buttons that use EXTI
 * https://www.youtube.com/watch?v=o0qhmXR5LD0
 */
void GPIO_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == PA1_Pin)
	{
		GPIO_UpdatePinStatus(&inputs, 0, HAL_GPIO_ReadPin(PA1_GPIO_Port, PA1_Pin));
	}
	else if(GPIO_Pin == PA2_Pin)
	{
		GPIO_UpdatePinStatus(&inputs, 1, HAL_GPIO_ReadPin(PA2_GPIO_Port, PA2_Pin));
	}
	else if(GPIO_Pin == PC13_Pin)
	{
		GPIO_UpdatePinStatus(&inputs, 2, HAL_GPIO_ReadPin(PC13_GPIO_Port, PC13_Pin));
	}
}

/*
 * STM32G071 HAL driver uses Rising and Falling callbacks where other STM32's use 1 callback.
 */
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
	GPIO_Callback(GPIO_Pin);
}

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
	GPIO_Callback(GPIO_Pin);
}
