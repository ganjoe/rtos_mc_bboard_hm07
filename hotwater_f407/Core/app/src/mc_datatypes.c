/*
 * mc_datatypes.c
 *
 *  Created on: 19.02.2021
 *      Author: daniel
 *
 */


#include "../mc_datatypes.h"


void mc_init_boardLedPwm(TD_MC_PARAMS* mcbench,TD_MC_PWM_PARAMS *pwm)
{
	mcbench->pwm = pwm;
	mcbench->pwm->speed = 64000000;
	mcbench->pwm->bits = 16;
	mcbench->pwm->duty_max = 0.9999;
	mcbench->pwm->duty_min = 0.00001;
	mcbench->pwm->freq_max = 0xFFFF;
	mcbench->pwm->freq_min = 0xF;
}

void mc_init_boardLedRamp(TD_MC_PARAMS* mcbench,RMPCNTL *ramp)
{
	mcbench->ramp = ramp;
	mcbench->ramp->gain = 1;
	mcbench->ramp->highlimit = 1;
	mcbench->ramp->lowlimit = -1.0;
	mcbench->ramp->timestep = 0.001;
	mcbench->ramp->RampStepLimit = 0.01;
}

TD_MC_PARAMS mcbench;
