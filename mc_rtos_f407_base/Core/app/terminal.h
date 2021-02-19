/*
 * terminal.h
 *
 *  Created on: Jul 7, 2020
 *      Author: daniel
 */

#ifndef INC_TERMINAL_H_
#define INC_TERMINAL_H_
#include "stdio.h"
#include "stdarg.h"
#include "main.h"
#include "usart.h"
#include "datatypes.h"
#include "queue.h"
#include "semphr.h"

/*-----------------------------------------------------------*/
//size for static td - do not change
#define CALLBACK_LEN		40

//array size for snprintf
#define UART_PRINTBUFFER 	64

//bytes send to uart every taskloop (DMA Transfer Buffer Size)
#define TX_BYTES_AT_ONCE	16

//HAL USART HANDLE
#define HUART &huart1
/*-----------------------------------------------------------*/
/*-----------------------------------------------------------*/
void dbase_StoreSD(TD_LINEOBJ *line);

void dbase_LoadQueue ( osMessageQueueId_t QueueHandle, TD_LINEOBJ *line);

//wrapper for vsnprintf. prints to queue
void term_qPrintf(osMessageQueueId_t QueueHandle, char *fmt, ...);

BaseType_t dBase_StoreQueue(osMessageQueueId_t QueueHandle, TD_LINEOBJ *line);

void dbase_Make(TD_LINEOBJ *line,
			const char* filename,
			const char* string,
			const char* header,
			const char* postfix,
			uint16_t* linenr,
			const char 	*fmt,
			... );
/*-----------------------------------------------------------*/
/*-----------------------------------------------------------*/
extern TD_TERMINAL btTerm;


#endif /* INC_TERMINAL_H_ */
