/*
 * UART_Handler.c
 *
 *  Created on: Jul 21, 2023
 *      Author: karl.yamashita
 */



#include "main.h"



/*
 * Description: Enables the HAL_UART_Receive_IT interrupt. Call before main while loop and in HAL_UART_RxCpltCallback
 */
void UART_EnableRxInterrupt(UartBufferStruct *msg)
{
	msg->rx.HAL_Status = HAL_UART_Receive_IT(msg->huart, &msg->rx.irqByte, 1);
}

/*
 * Description: If error flag then call UART_EnableRxInterrupt. Call from main while loop.
 *
 */
void UART_CheckRxIntError(UartBufferStruct *msg)
{
	if(msg->rx.HAL_Status != HAL_OK)
	{
		msg->rx.HAL_Status = HAL_OK;
		UART_EnableRxInterrupt(msg);
	}
}


/*
 * Description: Transmit any available messages. Call from main while loop
 */
int UART_TxMessage_IT(UartBufferStruct *msg)
{
	int status = 0;
	UartDataStruct *ptr;

	if(msg->tx.ptr.cnt_Handle)
	{
		ptr = &msg->tx.queue[msg->tx.ptr.index_OUT];

		if(HAL_UART_Transmit_IT(msg->huart, ptr->data, ptr->size) == HAL_OK)
		{
			RingBuff_Ptr_Output(&msg->tx.ptr, msg->tx.queueSize);
		}
	}

	return status;
}


/*
 * Example callback and uart instances
 */

/*
 *

UartBufferStruct lpuart1 =
{
	.huart = &hlpuart1,
	.rx.uartType = UART_ASCII,
	.rx.irqByte = 0,
	.rx.queueSize = UART_RX_MESSAGE_QUEUE_SIZE,
	.tx.queueSize = UART_TX_MESSAGE_QUEUE_SIZE
};

UartBufferStruct uart1 =
{
	.huart = &hlpuart1,
	.rx.uartType = UART_ASCII,
	.rx.irqByte = 0,
	.rx.queueSize = UART_RX_MESSAGE_QUEUE_SIZE,
	.tx.queueSize = UART_TX_MESSAGE_QUEUE_SIZE
};

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

*/
