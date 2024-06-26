/*
 * PollingRoutine.c
 *
 *  Created on: Oct 24, 2023
 *      Author: karl.yamashita
 *
 *
 *      Template for projects.
 *
 */


#include "main.h"

extern TimerCallbackStruct timerCallback;
extern  UartBufferStruct lpuart1;
extern UartBufferStruct uart1;

Flags_t flags = {0};


void PollingInit(void)
{
	TimerCallbackRegisterOnly(&timerCallback, LED_Toggle);
	//TimerCallbackTimerStart(&timerCallback, LED_Toggle, 500, TIMER_REPEAT);

	TimerCallbackRegisterOnly(&timerCallback, PB_Pressed);

	UART_EnableRxInterrupt(&uart1);
	UART_EnableRxInterrupt(&lpuart1);

	UART_NotifyUser(&lpuart1, "LPUART1 Ready", strlen("LPUART1 Ready"), true);
	UART_NotifyUser(&uart1, "UART1 Ready", strlen("UART1 Ready"), true);
}

void PollingRoutine(void)
{
	TimerCallbackCheck(&timerCallback);

	UART_Parse_1(&uart1);
	UART_Parse_lp1(&lpuart1);

	UART_CheckRxIntError(&uart1);
	UART_CheckRxIntError(&lpuart1);
}

// GNSS
void UART_Parse_1(UartBufferStruct *msg)
{
	UartDataStruct *ptr;
	char str[32] = {0};
	char msgCopy[512] = {0};

	if(UART_RxMessagePending(msg))
	{
		ptr = msg->rx.msgToParse;
		memcpy(&msgCopy, &ptr->data, strlen((char*)ptr->data));

		if(!NMEA_CalculateChecksum(msgCopy))
		{
			UART_NotifyUser(&lpuart1, "Checksum mismatch", strlen("Checksum mismatch"), true);
			return;
		}

		if(flags.parse) // parse data. We're parsing 3 messages here
		{
			if(strncmp((char*)ptr->data, "$GNVTG", strlen("$GNVTG")) == 0)
			{
				NMEA_GNVTG((char*)ptr->data);
			}
			else if(strncmp((char*)ptr->data, "$GNGLL", strlen("$GNGLL")) == 0)
			{
				NMEA_GNGLL((char*)ptr->data);
			}
			else if(strncmp((char*)ptr->data, "$GNRMC", strlen("$GNRMC")) == 0)
			{
				NMEA_GNRMC((char*)ptr->data);
			}
			// user can add more messages to parse
			else // unknown command
			{
				sprintf(str, "%s is not in the list to parse.", strtok((char*)ptr->data, ","));
				UART_NotifyUser(&lpuart1, str, strlen(str), true);
			}
		}
		else // pass through messages from NEO-M8N to VCP
		{
			UART_NotifyUser(&lpuart1, (char*)ptr->data, strlen((char*)ptr->data), false);
		}
	}
}

// VCP
void UART_Parse_lp1(UartBufferStruct *msg)
{
	UartDataStruct *ptr;

	if(UART_RxMessagePending(msg))
	{
		ptr = msg->rx.msgToParse;
		RemoveSpaces((char*)ptr->data);

		if(strncmp((char*)ptr->data, "setparse:1", strlen("setparse:1")) == 0)
		{
			flags.parse = 1;
		}
		else if(strncmp((char*)ptr->data, "setparse:0", strlen("setparse:0")) == 0) // setparse:0
		{
			flags.parse = 0;
		}
		else if(strncmp((char*)ptr->data, "setgoogle:1", strlen("setgoogle:1")) == 0)
		{
			flags.googleMaps = 1;
		}
		else if(strncmp((char*)ptr->data, "setgoogle:0", strlen("setgoogle:0")) == 0)
		{
			flags.googleMaps = 0;
		}
		UART_NotifyUser(&uart1, (char*)ptr->data, strlen((char*)ptr->data), false);
	}
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == B1_Pin)
	{
		if(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_SET)
		{
			TimerCallbackTimerStart(&timerCallback, PB_Pressed, 10, TIMER_NO_REPEAT);
		}
		else
		{
			TimerCallbackDisable(&timerCallback, PB_Pressed);
			TimerCallbackTimerStart(&timerCallback, LED_Toggle, 500, TIMER_REPEAT);
		}
	}
}

void LED_Toggle(void)
{
	HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
}

void PB_Pressed(void)
{
	TimerCallbackTimerStart(&timerCallback, LED_Toggle, 100, TIMER_REPEAT);
}
