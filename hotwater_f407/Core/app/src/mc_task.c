/*
 * mc_task.c
 *
 *  Created on: Feb 19, 2021
 *      Author: danie
 */

/* Includes für FreeRtos - Tasks und so */

#include "../mc_task.h"
#include "../mc_datatypes.h"
#include "../mc_pwm_if.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "adc.h"

/* 	Includes für Propelli	 */
/* Includes für Motorcontrol */

#include "../mc_ramp.h"
#include "../newCmdOrder.h"



void StartMcTask(void *argument)
{

	while(1)
	{

	}
/*
	mc_init_default(&mcbench);
	mc_init_bboard_hm07_boardLedPwm(&mcbench);
	mc_ramp_init(&mcbench.potiramp);
	mf_systick.timerspeed = pwm.speed;

	uint32_t buff1[3];

	HAL_ADC_Start_DMA(&hadc1, buff1 , 3);



	float timestep;

    while (1)
    	{

    	mc_timediff(&mf_systick, &timestep);

    		mcbench.potiramp.RampTimestep = timestep;

    	mc_ramp		(&mcbench.potiramp);

    		pwm.duty = mcbench.potiramp.SetpointValue;

		mc_setduty	(&mcbench);
    	}
    	*/

}

