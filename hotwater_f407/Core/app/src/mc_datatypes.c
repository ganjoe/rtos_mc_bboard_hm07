/*
 * mc_datatypes.c
 *
 *  Created on: 19.02.2021
 *      Author: danie
 */

#include "FreeRTOS.h"
#include "../mc_datatypes.h"
#include "../mc_pwm_if.h"
#include "../utils.h"
#include "cmsis_os2.h"
#include "../terminal.h"

extern osMessageQueueId_t myTxQueueHandle;

void mc_init_default(TD_MC_PARAMS* mcbench)
{
	mcbench->pwm.pwm_timer_speed = 64000000;
	mcbench->pwm.pwm_timer_bits = 16;
}

void mc_init_bboard_hm07_boardLedPwm(TD_MC_PARAMS* mcbench)
{

	mcbench->benchsetup = bb_boardled;	// debug mit leds für drehrichtung auf f407 blackboard
	mcbench->pwmmode = bcd6x;			//ein signal pro fet, u.a. für fettreiber
	mcbench->states = standby;			//u.a. für leuchtdioden

	mcbench->pwm.pwm_duty_max = 1.0;
	mcbench->pwm.pwm_duty_min = 0.0;
	mcbench->pwm.pwm_freq_max = 0xFFFF;	//TODO: abfrage ob es um min-bits kleiner ist als timer_speed
	mcbench->pwm.pwm_freq_min = 0xF;

}

void mc_init_bboard_hm07_boatblower(TD_MC_PARAMS* mcbench)
{

}

void mc_setfreq(uint32_t freq, TD_MC_PARAMS* mcbench)
	{
	uint32_t timertop = 0;

	if(utils_truncate_number_int((int*)freq, mcbench->pwm.pwm_freq_min,  mcbench->pwm.pwm_freq_max))
		term_qPrintf(myTxQueueHandle, "[mc_setfreq] freq truncated ! %d", timertop);

	timertop = mcbench->pwm.pwm_timer_speed / freq;

	if(utils_truncate_number_int((int*)timertop, 6, 2^(mcbench->pwm.pwm_timer_bits)))
		term_qPrintf(myTxQueueHandle, "[mc_setfreq] timertop truncated ! %d", timertop);

	mc_set_mcTimerTop(mcbench->pwm.pwm_timer_top);

	term_qPrintf(myTxQueueHandle, "[mc_setfreq] new: %d", timertop);

	}

void mc_setduty(float duty, TD_MC_PARAMS* mcbench)
	{
	uint32_t compare = 0;

	if(utils_truncate_number(&duty, -1, 1))
		term_qPrintf(myTxQueueHandle, "[mc_setduty] duty truncated ! %f", duty);

	/* das ist die wahre kraft des typedef enum */

	switch (mcbench->benchsetup)
		{
		case bb_boardled:
			{
			float setpoint;
			// timercompare berechnen
			setpoint = duty * (float)mcbench->pwm.pwm_timer_top;

			if (duty > 0)
				mc_pwm_bboard_led_1((uint32_t)setpoint);
			if (duty < 0)
				mc_pwm_bboard_led_2((uint32_t)setpoint);

			term_qPrintf(myTxQueueHandle, "[mc_setduty][bb_boardled] <duty/ticks>%f %d",duty, (uint32_t)setpoint);
			//store as  init command
			}
			break;
		case bb_hm7_blower:
			{
			term_qPrintf(myTxQueueHandle, "[mc_setduty][bb_hm7_blower] -pass");
			}
			break;
		default:
			{
			term_qPrintf(myTxQueueHandle, "[mc_setduty] mcbench default %d", mcbench->benchsetup);

			}
			break;

		}

	}

TD_MC_PARAMS mcbench;
