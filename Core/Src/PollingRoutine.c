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
extern UART_HandleTypeDef hlpuart1;
extern UART_HandleTypeDef huart1;

bool ParseMode = false;

// Init uart2 or lpuart1
UartBufferStruct lpuart1 =
{
	.huart = &hlpuart1,
	.rx.uartType = UART_ASCII,
	.rx.irqByte = 0,
	.rx.queueSize = UART_RX_MESSAGE_QUEUE_SIZE,
	.tx.queueSize = UART_TX_MESSAGE_QUEUE_SIZE,
	.rx.byteSize = UART_RX_BYTE_BUFFER_SIZE
};

UartBufferStruct uart1 =
{
	.huart = &huart1,
	.rx.uartType = UART_ASCII,
	.rx.irqByte = 0,
	.rx.queueSize = UART_RX_MESSAGE_QUEUE_SIZE,
	.tx.queueSize = UART_TX_MESSAGE_QUEUE_SIZE,
	.rx.byteSize = UART_RX_BYTE_BUFFER_SIZE
};

void PollingInit(void)
{
	TimerCallbackRegisterOnly(&timerCallback, LED_Toggle);
	TimerCallbackTimerStart(&timerCallback, LED_Toggle, 500, TIMER_REPEAT);

	TimerCallbackRegisterOnly(&timerCallback, PB_Pressed);

	UART_EnableRxInterrupt(&uart1);
	UART_EnableRxInterrupt(&lpuart1);

	NotifyUser(&lpuart1, "LPUART1 Ready", true);
	NotifyUser(&uart1, "UART1 Ready", true);
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

	if(UART_RxMessagePending(msg)) // not doing anything with data, just keeping the ring buffer pointers moving along
	{
		ptr = msg->rx.msgToParse;

		if(ParseMode) // parse data
		{
			if(strncmp((char*)ptr->data, "$GNVTG", strlen("$GNVTG")) == 0)
			{
				NMEA_VTG((char*)ptr->data);
			}
			else if(strncmp((char*)ptr->data, "$GNGLL", strlen("$GNGLL")) == 0)
			{
				NMEA_GLL((char*)ptr->data);
			}
			else // unknown command
			{

			}
		}
		else // show normal
		{
			NotifyUser(&lpuart1, (char*)ptr->data, false);
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
			ParseMode = 1;
		}
		else // setparse:0
		{
			ParseMode = 0;
		}
		NotifyUser(&uart1, (char*)ptr->data, false);
	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == uart1.huart)
	{
		UART_AddByteToBuffer(&uart1);
		UART_EnableRxInterrupt(&uart1);
	}
	else if(huart == lpuart1.huart)
	{
		UART_AddByteToBuffer(&lpuart1);
		UART_EnableRxInterrupt(&lpuart1);
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == uart1.huart)
	{
		UART_TxMessage_IT(&uart1);
	}
	else if(huart == lpuart1.huart)
	{
		UART_TxMessage_IT(&lpuart1);
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