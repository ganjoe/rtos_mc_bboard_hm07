/*
 * datatypes.c
 *
 *  Created on: Jan 15, 2021
 *      Author: danie
 */
#include "../datatypes.h"


#define HAL_SYSTICKTIMER TIM12


void modflag_init()
{

	  mf_systick.timerspeed = 168000000;
}

void modflag_ovf_callback()
    {
    ++mf_systick.ovf;
    }


/**
 * @note tickdiff timer muss der hal systick-timer sein
 * @note dieser wird von der codegenerierung initialisiert
 */
void mc_tickdiff(TD_MODFLAG *cnt)
    {
    cnt->systick = HAL_SYSTICKTIMER->CNT;
    cnt->ovf = cnt->ovf;
    cnt->systick = cnt->systick + (cnt->ovf * HAL_SYSTICKTIMER->ARR);
    cnt->newtick = cnt->systick;
    cnt->tickdiff = cnt->newtick - cnt->oldtick;
    cnt->oldtick = cnt->newtick;
    }

void mc_timediff(TD_MODFLAG *cnt)
    {
	mc_tickdiff(cnt);
	cnt->timestep = (float) (cnt->tickdiff) / (float) (cnt->timerspeed / (HAL_SYSTICKTIMER->PSC + 1));
    }

/*
 * @brief _weak funktion aus stm-hal.
 * @note muss gecallt werden da sonst hal-delay bzw. die firmware nicht funktioniert
 */
	void HAL_IncTick(void)
	{
	  uwTick += uwTickFreq;
	  modflag_ovf_callback();
	}

TD_MODFLAG mf_systick;
