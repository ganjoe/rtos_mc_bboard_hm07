/*
 * mc_pwm_if.h
 *
 *  Created on: 18.02.2021
 *      Author: pymd02
 *
 *       spezielles Low-Level-Interface
 *       stm-hal handles von hand anpassen
 *
 */

#ifndef INC_MC_PWM_IF_H_
#define INC_MC_PWM_IF_H_

#include "main.h"
	typedef enum
	{
	    cw_pwm = 1,
	    ccw_pwm,

	    cw_shaft,
	    ccw_shaft,
	    detect,
	}
	    EN_MC_MOTORSTATE;

typedef struct
{
	/**
	 * @brief Timerhandle für mc-pwm
	 * @note das handle ist in tim.h initialisiert
	 */
	TIM_HandleTypeDef htim;

	/**
	 * @brief takte pro sekunde
	 * @note offline
	 */
	uint32_t speed;

	/**
	 * @brief zählertiefe (z.B. 12bit)
	 */
	uint32_t bits;

	uint32_t top, prescaler;

	float duty,  freq;

	EN_MC_MOTORSTATE direction;
}
	TD_MC_PWM_PARAMS;



void pwm_init_timer_led1		(TD_MC_PWM_PARAMS* pwm);
void pwm_init_timer_led2		(TD_MC_PWM_PARAMS* pwm);
void pwm_init_timer_mc			(TD_MC_PWM_PARAMS *pwm);

void mc_pwm_led_update			(TD_MC_PWM_PARAMS* pwm);
EN_MC_MOTORSTATE mc_pwm_bcd_update	(TD_MC_PWM_PARAMS *pwm);

void pwm_calcfreq(TD_MC_PWM_PARAMS *pwm);



extern TD_MC_PWM_PARAMS pwm, pwm_led1;

#endif /* INC_MC_PWM_IF_H_ */
