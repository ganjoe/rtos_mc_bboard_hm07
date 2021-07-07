/*
 * terminal.c
 *
 *  Created on: Jan 2, 2021
 *      Author: daniel
 *
 *      23.04.21
 */
#include "../terminal.h"

/*--------freertos.c export---------*/
extern osMessageQueueId_t myRxQueueHandle;

extern osMessageQueueId_t myTxQueueHandle;

extern osSemaphoreId_t myFlagNewStringHandle;

extern osMessageQueueId_t mySDwriteBufferLineObjQueueHandle;

extern osSemaphoreId_t myCountNewCmdHandle;

extern osMessageQueueId_t myCmdLineObjQueueHandle;
/*--------/freertos.c export---------*/

uint8_t lReceivedValue[1];

/*-----------------------------------------------------------*/
void term_qPrintf	(osMessageQueueId_t QueueHandle, char *fmt, ...)
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
	    osMessageQueuePut(QueueHandle, &pbuffer[var], 0, osWaitForever);
	    // xQueueSendToBack(QueueHandle, &pbuffer[var], 0);
	    }
	}
    else
	{
	xQueueReset(QueueHandle);

	xQueueSendToBack(QueueHandle, "!F", 0);

	}
    }

void term_vprintLineObj(osMessageQueueId_t QueueHandle, TD_LINEOBJ *line)
    {
    term_qPrintf(QueueHandle, "%s, %s, %s, %s, %s;\r", line->timestring,
	    line->filename, line->header, line->string, line->postfix);
    }

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

/*___________________________________________________________*/

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
    {
    uint8_t byte_received = huart->Instance->DR;

    xQueueSendToBackFromISR(myRxQueueHandle, &byte_received, 0);

    if (byte_received == (uint8_t) 13)
	xSemaphoreGiveFromISR(myFlagNewStringHandle, 0);

    HAL_UART_Receive_DMA(HUART, (uint8_t*) &readbyte, 1);
    }

void StartRxTask(void *argument)
    {
    for (;;)
	{
	if (xSemaphoreTakeFromISR(myFlagNewStringHandle,0xffff) == pdPASS)
	    {
	    int ItemsLeft = uxQueueMessagesWaiting(myRxQueueHandle);

	    if (ItemsLeft)
		{
		TD_LINEOBJ lobj;
		char rxbuff[ItemsLeft];
		memset(rxbuff, '\0', ItemsLeft);

		for (int var = 0; var < ItemsLeft; ++var)
		    {
		    uint8_t pvBuffer = 0;
		    xQueueReceive(myRxQueueHandle, &pvBuffer, 0);
		    rxbuff[var] = pvBuffer;
		    }
		rxbuff[ItemsLeft - 1] = '\0';
		dbase_Make(&lobj, strdup("cmd"), strdup(rxbuff), 0, 0, 0, 0);
		dBase_StoreQueue(myCmdLineObjQueueHandle, &lobj);
		xSemaphoreGive(myCountNewCmdHandle);
		}
	    }

	}
    }

void StartTxTask(void *argument)
    {
    for (;;)
	{
	osStatus val = 0;
	val = osMessageQueueGet(myTxQueueHandle, &lReceivedValue, 0, 100);
	if (val == osOK)
	    {
	    HAL_UART_Transmit(&huart1, lReceivedValue, 1, 199);
	    }
	else
	    {


	    }
	}
    }

