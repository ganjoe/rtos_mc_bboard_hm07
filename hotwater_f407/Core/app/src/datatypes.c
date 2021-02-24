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

//Stm32F40n Timer 6 ist max 84Mhz, Timer 10/11 max 168Mhz


void mc_tickdiff(TD_MODFLAG *cnt)
    {
    uint64_t counter = TIM6->CNT;
    cnt->ovf = mf_systick.ovf;

    cnt->systick = counter + (cnt->ovf * TIM6->ARR);
    cnt->newtick = cnt->systick;
    cnt->tickdiff = cnt->newtick - cnt->oldtick;
    cnt->oldtick = cnt->newtick;

    }

void mc_timediff(TD_MODFLAG *cnt, float* timesec)
    {
	mc_tickdiff(cnt);

    if(timesec !=NULL)
	{
	*timesec = (float) cnt->timerspeed / TIM6->PSC;
	*timesec = (float)cnt->tickdiff / (*timesec);
	}
    }

TD_MODFLAG mf_systick;
