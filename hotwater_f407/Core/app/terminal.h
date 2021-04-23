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


#define HUART &huart1

void term_qPrintf(osMessageQueueId_t QueueHandle, char *fmt, ...);

void term_vprintLineObj(osMessageQueueId_t QueueHandle, TD_LINEOBJ *line);

#endif /* INC_TERMINAL_H_ */
