/*
 * terminal.h
 *
 *  Created on: Jul 7, 2020
 *      Author: daniel
 */

#ifndef INC_TERMINAL_H_
#define INC_TERMINAL_H_

#include "usart.h"
#include "datatypes.h"
#include "dbase.h"

const uint8_t* readbyte;

extern osMessageQueueId_t myTxQueueHandle;


#define HUART &huart1
#define TX_BYTES_AT_ONCE 128

void term_qPrintf(osMessageQueueId_t QueueHandle, char *fmt, ...);

void term_vprintLineObj(osMessageQueueId_t QueueHandle, TD_LINEOBJ *line);

void term_sendBuffer(osMessageQueueId_t QueueHandle, char* buffer, int buffersize);

//void term_sendBuffer(osMessageQueueId_t QueueHandle, char buffer, uint16_t buffersize);

#endif /* INC_TERMINAL_H_ */
