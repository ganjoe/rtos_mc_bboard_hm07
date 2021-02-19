/*
 * flowmeter.c
 *
 *  Created on: Jan 14, 2021
 *      Author: danie
 */
#include "main.h"

#include "FreeRTOS.h"
#include "semphr.h"
#include "timers.h"
#include "cmsis_os2.h"

#include "../flowmeter.h"

extern osSemaphoreId_t myFlagNewEdgeFlowHotHandle;
extern osTimerId_t myTimerFlowHotHandle;



void HAL_GPIO_EXTI_Callback(uint16_t Pin)
    {
    osSemaphoreRelease(myFlagNewEdgeFlowHotHandle);
    //xSemaphoreGiveFromISR(myFlagNewEdgeFlowHotHandle, 0);
    }



TD_FLOWMETER flowhot, flowcold;
