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
#include "main.h"
#include "tim.h"

//TODO: mit ganz vielen defines die targets auseinanderhalten

void mc_set_mcTimerTop(uint32_t period, uint32_t prescaler)
	{

		  htim3.Instance = TIM3;
		  htim3.Init.Prescaler = 0;
		  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
		  htim3.Init.Period = period;
		  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
		  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
		  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
		  {
			Error_Handler();
		  }

	}

/* Anzahl takte für eine Ton - Periode des Timers
 * Der einzige workbench-Timer hat mehrere channel bzw.
 * compares
 *
 */
static TIM_OC_InitTypeDef sConfigOC = {0};

void mc_pwm_bboard_led_1		(uint32_t setpoint)
	{
	if(! (sConfigOC.Pulse == setpoint ))
		{

		  sConfigOC.OCMode = TIM_OCMODE_PWM1;
		  sConfigOC.Pulse = setpoint;
		  sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
		  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
		  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
		  {
			Error_Handler();
		  }
		  HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_2);
		}

		//
	}

void mc_pwm_bboard_led_2		(uint32_t setpoint)
	{
	//init flag abfragen
	TIM_OC_InitTypeDef sConfigOC = {0};
	  sConfigOC.OCMode = TIM_OCMODE_PWM1;
	  sConfigOC.Pulse = setpoint;
	  sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
	  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_1);
		//
	}
