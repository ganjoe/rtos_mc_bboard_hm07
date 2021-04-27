/*
 * mc_datatypes.c
 *
 *  Created on: 19.02.2021
 *      Author: daniel
 *
 *	fertige parametersätze für vorhandene variablen
 */


#include "../mc_datatypes.h"


void mc_init_boardLedPwm(TD_MC_PWM_PARAMS *pwm)
{
    pwm->speed = 64000000;
    pwm->bits = 0xFFFF;
    pwm->duty_max = 0.9999;
    pwm->duty_min = 0.00001;
    pwm->freq_max = 0xFFFF;
    pwm->freq_min = 0xF;
}

void mc_init_BlowerPwm(TD_MC_PWM_PARAMS *pwm)
{
    pwm->speed = 64000000;
    pwm->bits = 0xFFFF;
    pwm->duty_max = 0.9999;
    pwm->duty_min = 0.00001;
    pwm->freq_max = 0xFFFF;
    pwm->freq_min = 0xF;
}

void mc_init_boardLedRamp(RMPCNTL* ramp)
{
    ramp->gain = 1;
    ramp->highlimit = 1;
    ramp->lowlimit = -1.0;
    ramp->timestep = 0.001;
    ramp->RampStepLimit = 0.01;
}

void mc_init_BlowerRamp(RMPCNTL* ramp)
{
    ramp->gain = 1;
    ramp->highlimit = 1;
    ramp->lowlimit = -1.0;
    ramp->timestep = 0.001;
    ramp->RampStepLimit = 0.01;
}

TD_MC_ADC_MATH math_shuntu, math_shuntv, math_emku, math_emkv;


TD_MC_PARAMS mcbench;
