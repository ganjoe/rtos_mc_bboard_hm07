/*
 * datatypes.h
 *
 *  Created on: Jan 2, 2021
 *      Author: danie
 */

#ifndef APP_DATATYPES_H_
#define APP_DATATYPES_H_

/*--stm hal--*/
#include "main.h"
#include "rtc.h"

/*--freertos--*/
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "cmsis_os2.h"
#include "cmsis_os.h"
#include "event_groups.h"

/*--c library--*/
#include "string.h"
#include "stdio.h"
#include "stdarg.h"
#include "stdlib.h"


#include "utils.h"
#include "timestep.h"

extern osEventFlagsId_t myEventMCtaskHandle;
extern osSemaphoreId_t myFlagNewDMAHandle;
extern osEventFlagsId_t myEventMCtaskHandle;

#define EventBit_InitDone 1<<0
#define EventBit_MCTaskDone 1<<1

#endif /* APP_DATATYPES_H_ */



