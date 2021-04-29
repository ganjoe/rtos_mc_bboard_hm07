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

	/**
	 * @brief timer overflow und compare event
	 * @note livewert für pwm-freq steuerung
	 */
	uint32_t top, prescaler, comp_u, comp_v, comp_w;

	/**
	 * @brief vorgaben für hw-limits
	 * @note 1/s
	 */
	uint32_t freq_max, freq_min;
	uint32_t duty_max, duty_min;

	/**
	 * @brief user - vorgaben für limiter
	 * @note dimensionslos, per unit
	 * @note mit vorzeichen für drehrichtung
	 */
	float pwm_duty_max, pwm_duty_min;

	/**
	 * @brief mc-loop dutycycle und pwm-frequenz
	 * @note normiert mit vorzeichen für drehrichtung
	 */
	float duty, duty_u, duty_v, duty_w, freq;
}
	TD_MC_PWM_PARAMS;


void pwm_init_timer_led1		(TD_MC_PWM_PARAMS* pwm);
void pwm_init_timer_led2		(TD_MC_PWM_PARAMS* pwm);
void pwm_init_timer_mc			(TD_MC_PWM_PARAMS *pwm);

void mc_pwm_led_update			(TD_MC_PWM_PARAMS* pwm);
void mc_pwm_bcd_update			(TD_MC_PWM_PARAMS *pwm);

void pwm_calcfreq(TD_MC_PWM_PARAMS *pwm);

void pwm_calcduty(TD_MC_PWM_PARAMS *pwm);

extern TD_MC_PWM_PARAMS pwm, pwm_led1;

#endif /* INC_MC_PWM_IF_H_ */
