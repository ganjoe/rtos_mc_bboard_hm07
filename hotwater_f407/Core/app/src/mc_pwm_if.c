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

void mc_pwm_update(TD_MC_PWM_PARAMS *pwm) {
	/**
	 * @note entspricht den macro __HAL_TIM_SET_AUTORELOAD usw.
	 * @note nicht verwendet um compilerwarnungen zu vermeiden
	 */
	pwm->htim.Instance->ARR = pwm->top;
	pwm->htim.Init.Prescaler = pwm->prescaler;

	switch (pwm->channelcount) {
		case mc_pwm_single:
			pwm->htim.Instance->CCR1 = pwm->comp_u; break;

		case mc_pwm_hbridge:
			pwm->htim.Instance->CCR1 = pwm->comp_u; break;
			pwm->htim.Instance->CCR2 = pwm->comp_v; break;

		case mc_pwm_threephase:
			pwm->htim.Instance->CCR1 = pwm->comp_u; break;
			pwm->htim.Instance->CCR2 = pwm->comp_v; break;
			pwm->htim.Instance->CCR3 = pwm->comp_w; break;

		default:			break;
	}
}

void mc_pwm_bboard_led1_init(TD_MC_PWM_PARAMS *pwm) {

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
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
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

}

void mc_pwm_bboard_led2_init(TD_MC_PWM_PARAMS *pwm) {
	HAL_TIM_PWM_Start_IT(&pwm->htim, TIM_CHANNEL_1);
}


TD_MC_PWM_PARAMS led1pwm;
