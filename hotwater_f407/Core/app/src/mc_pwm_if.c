/*
 * mc_pwm_if.c
 *
 *  Created on: 19.02.2021
 *      Author: ganjoe
 *
 *	ist die spezialisierung auf die mcu-hardware.
 *	die api ist "mc_datatyps" und/oder "mc_spezieller_motor"
 */


#include "../mc_pwm_if.h"

//TODO: mit ganz vielen defines die targets auseinanderhalten

void mc_set_mcTimerTop(uint32_t period)
{

}

/* Anzahl takte für eine Ton - Periode des Timers
 * Der einzige Timer hat mehrere channel bzw.
 * compares
 *
 */
void mc_pwm_bboard_led_1		(uint32_t setpoint)
{

}
void mc_pwm_bboard_led_2		(uint32_t setpoint)
{

}
