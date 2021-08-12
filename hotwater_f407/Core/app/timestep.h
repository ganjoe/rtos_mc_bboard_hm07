/*
 * timestep.h
 *
 *  Created on: Aug 12, 2021
 *      Author: danie
 */

#ifndef APP_TIMESTEP_H_
#define APP_TIMESTEP_H_

#include "main.h"

#define HAL_SYSTICKTIMER TIM12

//TIMERCLOCK / (HAL_SYSTICKTIMER->PSC +1)
#define HAL_TIMERSPEED 2000000



typedef struct
    {
    int divisor;
    uint32_t oldtick, systick, newtick, tickdiff;
    uint32_t timerspeed;	// für timediff berechnung, VOR prescaler
    }
    TD_TIMESTEP;

float timestep_si(TD_TIMESTEP* time);

extern TD_TIMESTEP mctime;

#endif /* APP_TIMESTEP_H_ */

