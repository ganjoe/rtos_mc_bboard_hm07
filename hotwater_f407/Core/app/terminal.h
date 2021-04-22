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
#include "usart.h"
#include "datatypes.h"
#include "cmsis_os2.h"
#include "queue.h"
#include "semphr.h"



    typedef struct
        {
        //TODO: replace with freeRTOS datatypes
        char byte_received;
        char* string_rx;
        unsigned int newString;

        /*
        //char* sep;
        //char* eoc;

        //char* string_tx;
        //int uart_buffer_tx_len;
         */

        int flag_newString, flag_newTransmission;
        }
        TD_TERMINAL;

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

void term_qPrintf(osMessageQueueId_t QueueHandle, char *fmt, ...);

void term_vprintLineObj(osMessageQueueId_t QueueHandle, TD_LINEOBJ *line);

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
