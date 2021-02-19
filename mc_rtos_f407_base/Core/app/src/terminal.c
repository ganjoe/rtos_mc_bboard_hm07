/*
 * terminal.c
 *
 *  Created on: Jan 2, 2021
 *      Author: danie
 */
#include "FreeRTOS.h"
#include "../terminal.h"
#include "../utils.h"
#include "usart.h"
#include "cmsis_os2.h"
#include "string.h"
#include "rtc.h"

/*--------private exportet prototypes---------*/

extern osMessageQueueId_t myRxQueueHandle;

extern osMessageQueueId_t myTxQueueHandle;

extern osSemaphoreId_t myFlagNewStringHandle;

extern osMessageQueueId_t mySDwriteBufferLineObjQueueHandle;

/*----------private local code----------------*/
BaseType_t dBase_StoreQueue(osMessageQueueId_t QueueHandle, TD_LINEOBJ *line)
    {
    return xQueueSend(QueueHandle, line, 2000);
    }
/*___________________________________________________________*/
void dBase_addTimeStr(TD_LINEOBJ *line)
    {
    RTC_TimeTypeDef rtc =
	{
	0
	};
    RTC_DateTypeDef rtd =
	{
	0
	};
    // muss immer zusammen gecallt werden.
    // rtc taktquelle in cube einstellen und funktion prüfen
    HAL_RTC_GetTime(&hrtc, &rtc, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &rtd, RTC_FORMAT_BIN);
    sprintf(line->timestring, "%2d.%2d. %2d:%2d:%2d", rtd.Date, rtd.Month,rtc.Hours, rtc.Minutes, rtc.Seconds);

    }
/*___________________________________________________________*/
void dbase_LoadQueue(osMessageQueueId_t QueueHandle, TD_LINEOBJ *line)
    {
    xQueueReceive(QueueHandle, line, ( portTickType ) 10);
    }
/*-----------------------------------------------------------*/
void term_qPrintf(osMessageQueueId_t QueueHandle, char *fmt, ...)
    {
    int ItemsLeft = uxQueueSpacesAvailable(QueueHandle);

    if (ItemsLeft)
	{
	va_list argp;

	va_start(argp, fmt);

	char pbuffer[UART_PRINTBUFFER];

	int bytesWrote = 0;

	bytesWrote = vsnprintf(pbuffer, UART_PRINTBUFFER, fmt, argp);

	va_end(argp);

	utils_truncate_number_int(&bytesWrote, 0, ItemsLeft);

	for (int var = 0; var < bytesWrote; ++var)
	    {
	    osMessageQueuePut(QueueHandle,  &pbuffer[var], 0, osWaitForever);
	   // xQueueSendToBack(QueueHandle, &pbuffer[var], 0);
	    }
	}
    else
	{
	xQueueReset(QueueHandle);

	xQueueSendToBack(QueueHandle, "!F", 0);

	}
    }
/*-----------------------------------------------------------*/
void dbase_Make(TD_LINEOBJ *line,const char *filename, const char *string,
	const char *header, const char *postfix, uint16_t *linenr, const char *fmt, ...)
    {
    char pbuffer[TD_LINEOBJ_MAX_SSIZE];

    dBase_addTimeStr(line);

    int len = 0;

    // no vsprint paramlist and already formatet string is given
    /*-----------------------------------------------------------*/
    if ((fmt == 0) & (string != 0))
	{
	snprintf(pbuffer, TD_LINEOBJ_MAX_SSIZE, string);
	strcpy(line->string, pbuffer);
	}
    //  printing to lineobject with given vsprintf format string
    /*-----------------------------------------------------------*/
    if ((fmt != 0) & (string == 0))
	{
	memset(pbuffer, 1, TD_LINEOBJ_MAX_SSIZE);
	va_list argp;
	va_start(argp, fmt);
	vsnprintf(pbuffer, UART_PRINTBUFFER, fmt, argp);
	strcpy(line->string, pbuffer);
	va_end(argp);
	}
    // printing double value with default format
    /*-----------------------------------------------------------*/
    if ((fmt == 0) & (string == 0))
	{
	snprintf(line->string, UART_PRINTBUFFER, "%.4g", line->value);
	}
    //
    /*-----------------------------------------------------------*/
    if (filename == 0)
	{
	snprintf( pbuffer, 2, " ");
	strcpy( line->filename, pbuffer);
	}
    else
	{
	len = strlen(filename);
	utils_truncate_number_int(&len, 0, TD_LINEOBJ_MAX_SSIZE);
	snprintf( pbuffer, len+1 , filename);
	strcpy( line->filename, pbuffer);
	}
    //
    /*-----------------------------------------------------------*/
    if (header == 0)
	{
	 snprintf( pbuffer, 2, " ");
	 strcpy( line->header, pbuffer);
	}
    else
	{
	len = strlen(header);
	utils_truncate_number_int(&len, 0, TD_LINEOBJ_MAX_HEADERSIZE);
	snprintf( pbuffer, len+1, header);
	 //strcpy( line->header, pbuffer);
	 strcpy( line->header, pbuffer);
	}
    //
    /*-----------------------------------------------------------*/
    if (postfix == 0)
	{
	 snprintf( pbuffer, 2, " ");
	 strcpy( line->postfix, pbuffer);
	}
    else
	{
	len = strlen(postfix);
	utils_truncate_number_int(&len, 0, TD_LINEOBJ_MAX_POSTFIXSIZE);
	snprintf( pbuffer, len+1, postfix);
	strcpy( line->postfix, pbuffer);
	}
    /*-----------------------------------------------------------*/

    // dont overwrite in case of static usage
    if (linenr != 0)
	line->linenr = *linenr;


    // dont overwrite in case of static usage
    if (linenr != 0)
	line->linenr = *linenr;

    }

/*-----------------------------------------------------------*/
void term_vprintLineObj(osMessageQueueId_t QueueHandle, TD_LINEOBJ *line)
    {
    term_qPrintf(QueueHandle, "%s, %s, %s, %s, %s;\r",
			    line->timestring,
			    line->filename,
			    line->header,
			    line->string,
			    line->postfix);
    }
void dbase_StoreSD(TD_LINEOBJ *line)
    {
    //osMessageQueuePut(mySDwriteBufferLineObjQueueHandle, line, 0, osWaitForever);
    }
/*-----------------------------------------------------------*/
void term_lol_sendQueue(osMessageQueueId_t QueueHandle)
    {
 /*   UBaseType_t ItemsLeft = uxQueueMessagesWaiting(QueueHandle);
    if (ItemsLeft)
	{
	//limits the numbers of bytes to send
	utils_truncate_number_int((int*) ItemsLeft, 0, TX_BYTES_AT_ONCE);
	//
	uint8_t dmaBuff[ItemsLeft];
	//copy bytes that are send at once to temp buffer
	for (int var = 0; var < ItemsLeft; ++var)
	    {
	    //should receive from front
	    uint8_t lReceivedValue;

	    xQueueReceive(QueueHandle, &lReceivedValue, 0);

	    dmaBuff[var] = lReceivedValue;
	    }
	//transmission time[s]: 10 bit / n baud

	HAL_UART_Transmit_DMA(&huart1, dmaBuff, ItemsLeft);

	uint32_t transmission_time = 10 * ItemsLeft * 1000	/ huart1.Init.BaudRate;



	}
	*/
    }
/*-----------------------------------------------------------*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
    {
    uint8_t byte_received = huart->Instance->DR;

    xQueueSendToBackFromISR(myRxQueueHandle, &byte_received, 0);

    if (byte_received == (uint8_t) 13)
	xSemaphoreGiveFromISR(myFlagNewStringHandle, 0);

    HAL_UART_Receive_DMA(&huart1, (uint8_t*) &readbyte, 1);
    }

TD_TERMINAL btTerm;
