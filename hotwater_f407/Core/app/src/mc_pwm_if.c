/*
 * mc_pwm_if.c
 *
 *  Created on: 19.02.2021
 *      Author: ganjoe
 *  Update 22.4.21
 *
 */

#include "../mc_pwm_if.h"
#include "stm32f4xx_hal.h"
#include "tim.h"

   /* PWM mode 2 = Clear on compare match */
   /* PWM mode 1 = Set on compare match */

EN_MC_MOTORSTATE mc_pwm_bcd_update(TD_MC_PWM_PARAMS *pwm)
/**
 * @note entspricht den macro __HAL_TIM_SET_AUTORELOAD usw.
*/
    {
	pwm_calcfreq(pwm);

	pwm->htim.Instance->PSC = pwm->prescaler;

	uint32_t reload = (float)(pwm->htim.Instance->ARR)* fabs(pwm->duty);

	pwm->htim.Instance->ARR = pwm->top;


	if (pwm->duty > 0)
	    {

	    pwm->htim.Instance->CCR1 = reload;
	    pwm->htim.Instance->CCR2 = 0x0;
	    HAL_GPIO_WritePin(enable_u_GPIO_Port, enable_u_Pin, 0);
	    HAL_GPIO_WritePin(enable_v_GPIO_Port, enable_v_Pin, 1);
	    return cw_pwm;
	    }
	if (pwm->duty < 0)
	    {

	    pwm->htim.Instance->CCR2 = reload;
	    pwm->htim.Instance->CCR1 = 0x0;
	    HAL_GPIO_WritePin(enable_u_GPIO_Port, enable_u_Pin, 1);
	    HAL_GPIO_WritePin(enable_v_GPIO_Port, enable_v_Pin, 0);
	    return ccw_pwm;
	    }

	if (pwm->duty == 0)
	    {
	    mc_set_lowside(pwm, brake_highz);
	    }
	return detect;


    }

void pwm_calcfreq(TD_MC_PWM_PARAMS *pwm)
{
	while (1)
		{
		pwm->top = pwm->speed/ (uint32_t)pwm->freq / (pwm->prescaler + 1) /2;

		if (pwm->top < pwm->bits)
		    break;
		else
		    pwm->prescaler++;
		}
}

void pwm_init_timer_mc(TD_MC_PWM_PARAMS *pwm)
    {

    /* USER CODE BEGIN TIM3_Init 0 */
    pwm->htim = htim3;

    HAL_TIM_PWM_Start_IT(&pwm->htim, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start_IT(&pwm->htim, TIM_CHANNEL_2);

    }

void mc_set_lowside(TD_MC_PWM_PARAMS *thispwm, EN_MC_MOTORSTATE state)
    {
    switch (state)
	{
	case brake_lowside:
	    thispwm->htim.Instance->CCR1 = 0x0;
	    thispwm->htim.Instance->CCR2 = 0x0;
	    HAL_GPIO_WritePin(enable_v_GPIO_Port, enable_u_Pin, 1);
	    HAL_GPIO_WritePin(enable_v_GPIO_Port, enable_v_Pin, 1);
	    break;
	case brake_highside:
	    thispwm->htim.Instance->CCR1 = 0xFFFF;
	    thispwm->htim.Instance->CCR2 = 0xFFFF;
	    HAL_GPIO_WritePin(enable_v_GPIO_Port, enable_u_Pin, 0);
	    HAL_GPIO_WritePin(enable_v_GPIO_Port, enable_v_Pin, 0);
	    break;
	case brake_highz:
	    thispwm->htim.Instance->CCR1 = 0x0;
	    thispwm->htim.Instance->CCR2 = 0x0;
	    HAL_GPIO_WritePin(enable_v_GPIO_Port, enable_u_Pin, 0);
	    HAL_GPIO_WritePin(enable_v_GPIO_Port, enable_v_Pin, 0);
	    break;
	}
    }


TD_MC_PWM_PARAMS pwm;
