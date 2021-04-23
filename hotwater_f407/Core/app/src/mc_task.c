/*
 * mc_task.c
 *
 *  Created on: Feb 19, 2021
 *      Author: danie
 */

/* Includes für FreeRtos - Tasks und so */

#include "../mc_datatypes.h"
#include "../mc_task.h"
#include "../mc_pwm_if.h"
#include "../mc_ramp.h"

void StartMcTask(void *argument)
{
/**
 * @brief Hardware Inits
 */
    pwm_init_bboard_led1	(&pwm);

/**
 * @brief Setup für Motorsitzung
 * @note könnte auch von den spezialisierten
 * @note mc_init_ funktionen gesetzt werden, ist so aber anschaulicher
 */
    mcbench.pwm = &pwm;
    mcbench.ramp = &rampe;
   // pwm_init_bboard_led2(&pwm);

/**
 * @brief Motor control Inits
 * @note kann on the fly gecallt werden
 */
    mc_init_boardLedPwm(&mcbench);
    mc_init_boardLedRamp(&mcbench);

	while(1)
	{
	    osDelay(1);

	    mc_timediff(&mf_systick);

	    mcbench.ramp->timestep = mf_systick.timestep;

	    mc_ramp		(&mcbench.ramp);

	    mcbench.pwm->duty = mcbench.ramp->Setpoint;

	   // mc_setduty	(&mcbench);

	}



}

