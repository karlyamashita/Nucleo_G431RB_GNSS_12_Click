/*
 * UART_Handler.c
 *
 *  Created on: Jul 21, 2023
 *      Author: karl.yamashita
 */



#include "main.h"


extern UART_HandleTypeDef hlpuart1;
extern UART_HandleTypeDef huart1;




// Init uart2 or lpuart1
UartBufferStruct lpuart1 =
{
	.huart = &hlpuart1,
	.rx.uartType = UART_ASCII,
	.rx.irqByte = 0,
	.rx.queueSize = UART_RX_QUEUE_SIZE,
	.tx.queueSize = UART_TX_QUEUE_SIZE,
	.rx.dataSize = UART_RX_DATA_SIZE,
	.rx.uartIRQ_Callback = LED_Toggle
};

UartBufferStruct uart1 =
{
	.huart = &huart1,
	.rx.uartType = UART_ASCII,
	.rx.irqByte = 0,
	.rx.queueSize = UART_RX_QUEUE_SIZE,
	.tx.queueSize = UART_TX_QUEUE_SIZE,
	.rx.dataSize = UART_RX_DATA_SIZE,
};


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
 * Description: Transmit any available messages.
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

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == uart1.huart)
	{
		UART_Rx_IRQ(&uart1);
	}
	else if(huart == lpuart1.huart)
	{
		UART_Rx_IRQ(&lpuart1);
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart == uart1.huart)
	{
		UART_Tx_IRQ(&uart1);
	}
	else if(huart == lpuart1.huart)
	{
		UART_Tx_IRQ(&lpuart1);
	}
}

void UART_NotifyUser(UartBufferStruct *msg, char *str, uint32_t size, bool lineFeed)
{
    uint8_t strMsg[UART_TX_DATA_SIZE] = {0};

    memcpy(&strMsg, str, size);

    if(lineFeed == true)
    {
    	strcat((char*)strMsg, "\r\n");
    	size += 2;
    }

    UART_TX_AddDataToBuffer(msg, strMsg, size);

    UART_TxMessage_IT(msg);
}

void UART_Rx_IRQ(UartBufferStruct *msg)
{
	UART_AddByteToBuffer(msg);
	UART_EnableRxInterrupt(msg);
	if(msg->rx.uartIRQ_Callback != NULL)
	{
		msg->rx.uartIRQ_Callback();
	}
}

void UART_Tx_IRQ(UartBufferStruct *msg)
{
	UART_TxMessage_IT(msg);
	if(msg->tx.uartIRQ_Callback != NULL)
	{
		msg->tx.uartIRQ_Callback();
	}
}

