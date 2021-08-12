/*
 * timestep.c
 *
 *  Created on: Aug 12, 2021
 *      Author: danie
 */

#include "../timestep.h"

uint32_t timestep_ovf;

float timestep_si(TD_TIMESTEP* time)
    {

    time->systick = HAL_SYSTICKTIMER->CNT + (timestep_ovf * HAL_SYSTICKTIMER->ARR);
    time->newtick = time->systick;
    time->tickdiff = time->newtick - time->oldtick;
    time->oldtick = time->newtick;
    time->timestep = time->tickdiff / (float)HAL_TIMERSPEED * 2;
    return  time->timestep;
    }


void HAL_IncTick(void)
{
  uwTick += uwTickFreq;
  timestep_ovf++;
 }

TD_TIMESTEP mctime;
