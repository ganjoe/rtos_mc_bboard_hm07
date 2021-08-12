/*
 * datatypes.c
 *
 *  Created on: Jan 15, 2021
 *      Author: danie
 */
#include "../datatypes.h"



/**
 * @note tickdiff timer muss der hal systick-timer sein
 * @note dieser wird von der codegenerierung initialisiert

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

 */





