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

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <ctype.h>
#define Nop() asm(" NOP ")

*/

typedef struct
{
	bool parse;
	bool googleMaps;
}Flags_t;

void PollingInit(void);
void PollingRoutine(void);

void UART_Parse_1(UartBufferStruct *msg);
void UART_Parse_lp1(UartBufferStruct *msg);
void LED_Toggle(void);
void PB_Pressed(void);


#endif /* INC_POLLINGROUTINE_H_ */
