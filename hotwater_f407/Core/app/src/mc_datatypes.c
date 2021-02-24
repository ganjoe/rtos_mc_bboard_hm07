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
	mcbench->pwm.pwm_timer_speed = 84000000;
	mcbench->pwm.pwm_timer_bits = 16;
}

void mc_init_bboard_hm07_boardLedPwm(TD_MC_PARAMS* mcbench)
{

	mcbench->benchsetup = bb_boardled;	// debug mit leds für drehrichtung auf f407 blackboard
	mcbench->pwmmode = bcd6x;			//ein signal pro fet, u.a. für fettreiber
	mcbench->states = standby;			//u.a. für leuchtdioden

	mcbench->pwm.pwm_duty_max = 0.9999;
	mcbench->pwm.pwm_duty_min = 0.00001;
	mcbench->pwm.pwm_freq_max = 0xFFFF;	//TODO: abfrage ob es um min-bits kleiner ist als timer_speed
	mcbench->pwm.pwm_freq_min = 0xF;
	mcbench->flag_init_wb = 1;

}

void mc_init_bboard_hm07_boatblower(TD_MC_PARAMS* mcbench)
{

}

void mc_setfreq(uint32_t freq, TD_MC_PARAMS* mcbench)
	{
	uint32_t timertop = 0;

	if(utils_truncate_number_int((int*)&freq, mcbench->pwm.pwm_freq_min,  mcbench->pwm.pwm_freq_max))
		{
		term_qPrintf(myTxQueueHandle, "\r[mc_setfreq] freq truncated ! %d", timertop);
		}

	mcbench->pwm.pwm_timer_top = mcbench->pwm.pwm_timer_speed / freq;

	if(utils_truncate_number_int((int*)&mcbench->pwm.pwm_timer_top, 6, powl(2, mcbench->pwm.pwm_timer_bits)))
		{
		term_qPrintf(myTxQueueHandle, "\r[mc_setfreq] timertop truncated ! %d", timertop);
		}

	mc_set_mcTimerTop(mcbench->pwm.pwm_timer_top, 0);		// null ist kein prescaler
	uint32_t newfreq = (mcbench->pwm.pwm_timer_speed / mcbench->pwm.pwm_timer_top) +1;
	term_qPrintf(myTxQueueHandle, "\r[mc_setfreq] <new>: %d [Hz]", newfreq);
	term_qPrintf(myTxQueueHandle, "\r[mc_set_mcTimerTop] <new|max>: %.0f | %.0f", (float)mcbench->pwm.pwm_timer_top, (float)powl(2, mcbench->pwm.pwm_timer_bits));
	}

void mc_setduty(TD_MC_PARAMS* mcbench)
	{
	utils_truncate_number(&mcbench->pwm.pwm_duty, -1, 1);

	/* das ist die wahre kraft des typedef enum */
	switch (mcbench->benchsetup)
		{
		case bb_boardled:
			{
			float setpoint;
			// timercompare berechnen
			if(! mcbench->pwm.pwm_timer_top)
				{
				//term_qPrintf(myTxQueueHandle, "\r[mc_setduty] set freq first ! ");
				break;
				}
			setpoint = (mcbench->pwm.pwm_duty) * (float)mcbench->pwm.pwm_timer_top;
			//term_qPrintf(myTxQueueHandle, "\r[mc_setduty][mc_pwm_bboard_led_n] <duty|pulse|pe1riod>: %.0f",  setpoint);

			if (mcbench->pwm.pwm_duty > 0)
				{
				utils_truncate_number(&mcbench->pwm.pwm_duty, mcbench->pwm.pwm_duty_min, mcbench->pwm.pwm_duty_max);
				mc_pwm_bboard_led_1((uint32_t)setpoint);
				mc_pwm_bboard_led_2((uint32_t)0);

				}

			if (mcbench->pwm.pwm_duty < 0)
				{
				utils_truncate_number(&mcbench->pwm.pwm_duty, mcbench->pwm.pwm_duty_min * -1, mcbench->pwm.pwm_duty_max * -1);
				setpoint *=-1;
				mc_pwm_bboard_led_1((uint32_t)0);
				mc_pwm_bboard_led_2((uint32_t)setpoint);

				}

			//store as  init command
			}
			break;

		case bb_hm7_blower:
			{
			term_qPrintf(myTxQueueHandle, "\r[mc_setduty][bb_hm7_blower] -pass");
			}
			break;
		default:
			{
			term_qPrintf(myTxQueueHandle, "\r[mc_setduty] mcbench default %d", mcbench->benchsetup);

			}
			break;

		}

	}

TD_MC_PARAMS mcbench;
