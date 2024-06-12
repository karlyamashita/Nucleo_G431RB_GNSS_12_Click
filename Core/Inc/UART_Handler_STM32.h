/*
 * UART_Handler.h
 *
 *  Created on: Jul 21, 2023
 *      Author: karl.yamashita
 */

#ifndef INC_UART_HANDLER_H_
#define INC_UART_HANDLER_H_


void UART_EnableRxInterrupt(UartBufferStruct *msg);
void UART_CheckRxIntError(UartBufferStruct *msg);
int UART_TxMessage_IT(UartBufferStruct *msg);

void UART_NotifyUser(UartBufferStruct *msg, char *str, uint32_t size, bool lineFeed);

void UART_Rx_IRQ(UartBufferStruct *msg);
void LPUART1_Rx_IRQ(void);
void UART_Tx_IRQ(UartBufferStruct *msg);
void LPUART1_Tx_IRQ(void);


#endif /* INC_UART_HANDLER_H_ */
