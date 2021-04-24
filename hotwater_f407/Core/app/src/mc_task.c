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
#include "../newCmdOrder.h"

void StartMcTask(void *argument)
{
/**
 * @brief propelli/modflag
 */

	modflag_init();
	cmd_init_callbacks(&newcmd);

/**
 * @brief Setup für Motorsitzung
 */

    mcbench.benchsetup = en_mode_led;
		pwm_init_bboard_led1	(&pwm);
		mc_init_boardLedPwm		(&pwm);
		mc_init_boardLedRamp	(&rampe);

    mcbench.pwm = &pwm;
    mcbench.ramp = &rampe;


/**
 * @brief Motor control Inits
 * @note kann on the fly gecallt werden
 */


	while(1)
	{
	    osDelay(1);

	    mc_timediff(&mf_systick);

	    mcbench.pwm->freq = 1000;

	    mcbench.ramp->Target = 0.1;

	    mcbench.ramp->gain = 0.4;

	    mcbench.ramp->timestep = mf_systick.timestep;

	    mc_ramp		(&rampe);

	    mcbench.pwm->duty = mcbench.ramp->Setpoint;

	    mc_pwm_update(mcbench.pwm);


	}



}

