/*
 * datatypes.c
 *
 *  Created on: Jan 15, 2021
 *      Author: danie
 */

#include "../datatypes.h"
#include "main.h"


void modflag_ovf_callback()
    {
    ++mf_systick.ovf;
    }

//TD_MODFLAG_TIMERSPEED


uint64_t modflag_tickdiff(TD_MODFLAG *cnt)
    {
    uint64_t counter = TIM6->CNT;
    cnt->ovf = mf_systick.ovf;

    cnt->systick = counter + (cnt->ovf * TIM6->ARR);
    cnt->newtick = cnt->systick;
    cnt->tickdiff = cnt->newtick - cnt->oldtick;
    cnt->oldtick = cnt->newtick;
    return cnt->tickdiff;
    }

void modflag_timediff(TD_MODFLAG *cnt, double* timesec)
    {
    uint64_t counter = TIM6->CNT;
    cnt->ovf = mf_systick.ovf;

    cnt->systick = counter + (cnt->ovf * TIM6->ARR);
    cnt->newtick = cnt->systick;
    cnt->tickdiff = cnt->newtick - cnt->oldtick;
    cnt->oldtick = cnt->newtick;

    if(timesec !=NULL)
	{
	*timesec =  (double)TD_MODFLAG_TIMERSPEED / TIM6->PSC;
	*timesec = (double)cnt->tickdiff / (*timesec);
	}
    }

TD_MODFLAG mf_systick;
