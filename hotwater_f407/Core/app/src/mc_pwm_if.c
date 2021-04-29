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



void mc_pwm_led_update(TD_MC_PWM_PARAMS *pwm) {
	/**
	 * @note entspricht den macro __HAL_TIM_SET_AUTORELOAD usw.
	 * @note verwenden um compilerwarnungen zu vermeiden
	 * @note center-aligned ist nur möglich wenn die setter
	 * @note den output status pollen und bei low setzen
	 */

	/* pu -> si */
	pwm_calcfreq(pwm);
	pwm_calcduty(pwm);

	pwm->htim.Instance->ARR = pwm->top;
	pwm->htim.Instance->PSC = pwm->prescaler;

	/**
	 * @brief low level pwm setter
	 *
	 * @note das hardware preload kann dies nicht, da nur bei overflow geupdated wird
	 */
	pwm->htim.Instance->CCR1 = pwm->comp_u;
}

void mc_pwm_bcd_update(TD_MC_PWM_PARAMS *pwm)
    {
	pwm_calcfreq(pwm);
	pwm->htim.Instance->ARR = pwm->top;
	pwm->htim.Instance->PSC = pwm->prescaler;
	uint32_t reload = (float)(pwm->htim.Instance->ARR)* fabs(pwm->duty);


	if (pwm->duty > 0)
	    {
	    pwm->htim.Instance->CCR1 = reload;
	    pwm->htim.Instance->CCR2 = 0x0;
	    HAL_GPIO_WritePin(enable_u_GPIO_Port, enable_u_Pin, 0);
	    HAL_GPIO_WritePin(enable_v_GPIO_Port, enable_v_Pin, 1);
	    }
	if (pwm->duty < 0)
	    {
	    pwm->htim.Instance->CCR2 = reload;
	    pwm->htim.Instance->CCR1 = 0x0;
	    HAL_GPIO_WritePin(enable_u_GPIO_Port, enable_u_Pin, 1);
	    HAL_GPIO_WritePin(enable_v_GPIO_Port, enable_v_Pin, 0);
	    }
	if (pwm->duty == 0)
	    {
	    pwm->htim.Instance->CCR2 = 0x0;
	    pwm->htim.Instance->CCR1 = 0x0;
	    }


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

void pwm_calcduty(TD_MC_PWM_PARAMS *pwm)
{
	pwm->comp_u = pwm->duty * (float)pwm->top;
}


void pwm_init_timer_led1(TD_MC_PWM_PARAMS *pwm) {

	__HAL_RCC_TIM13_CLK_ENABLE();

	TIM_OC_InitTypeDef sConfigOC = { 0 };

	pwm->htim.Instance = TIM13;
	pwm->htim.Init.Prescaler = 0;
	pwm->htim.Init.CounterMode = TIM_COUNTERMODE_UP;
	pwm->htim.Init.Period = 65535;
	pwm->htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	pwm->htim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (HAL_TIM_Base_Init(&pwm->htim) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_TIM_PWM_Init(&pwm->htim) != HAL_OK) {
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 33333;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&pwm->htim, &sConfigOC, TIM_CHANNEL_1)
			!= HAL_OK) {
		Error_Handler();
	}

	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitStruct.Pin = GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF9_TIM13;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	HAL_TIM_PWM_Start_IT(&pwm->htim, TIM_CHANNEL_1);

}

void pwm_init_timer_led2(TD_MC_PWM_PARAMS *pwm)
{
	//HAL_TIM_PWM_Start_IT(&pwm->htim, TIM_CHANNEL_1);
}

void pwm_init_timer_mc(TD_MC_PWM_PARAMS *pwm)
    {

    /* USER CODE BEGIN TIM3_Init 0 */
    pwm->htim = htim3;

    HAL_TIM_PWM_Start_IT(&pwm->htim, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start_IT(&pwm->htim, TIM_CHANNEL_2);

    }

TD_MC_PWM_PARAMS pwm, pwm_led1;
