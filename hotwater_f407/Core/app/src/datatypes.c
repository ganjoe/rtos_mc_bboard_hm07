/*
 * datatypes.c
 *
 *  Created on: Jan 15, 2021
 *      Author: danie
 */
#include "../datatypes.h"


void modflag_init()
{
	  TIM_HandleTypeDef htim11;
	  htim11.Instance = TIM11;
	  htim11.Init.Prescaler = 0;
	  htim11.Init.CounterMode = TIM_COUNTERMODE_UP;
	  htim11.Init.Period = 65535;
	  htim11.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	  htim11.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	  if (HAL_TIM_Base_Init(&htim11) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  mf_systick.timerspeed = 168E6;
}

void modflag_ovf_callback()
    {
    ++mf_systick.ovf;
    }

//Stm32F40n Timer 6 ist max 84Mhz, Timer 10/11 max 168Mhz


void mc_tickdiff(TD_MODFLAG *cnt)
    {
    cnt->systick = TIM11->CNT;
    cnt->ovf = cnt->ovf;
    cnt->systick = cnt->systick + (cnt->ovf * TIM11->ARR);
    cnt->newtick = cnt->systick;
    cnt->tickdiff = cnt->newtick - cnt->oldtick;
    cnt->oldtick = cnt->newtick;
    }

void mc_timediff(TD_MODFLAG *cnt)
    {
	mc_tickdiff(cnt);
	cnt->timestep = (float) (cnt->tickdiff) / (float) (cnt->timerspeed / (TIM11->PSC + 1));
    }
/*
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{

}
*/
TD_MODFLAG mf_systick;
