/*
 * mc_task.c
 *
 *  Created on: Feb 19, 2021
 *      Author: danie
 */

/* Includes für FreeRtos - Tasks und so */


#include "../mc_config.h"
#include "../mc_task.h"
#include "fatfs.h"
#include "tim.h"
#include "../datatypes.h"

extern osSemaphoreId_t myFlagNewDMAHandle;

void McTask()
    {
	{
	taskdoneflag = 1;
	    HAL_GPIO_TogglePin(test_GPIO_Port, test_Pin);

	    switch (pwm.direction)
		{
		case cw_pwm:
		mc_adc_CircBuffDemultiplex(&adc_1_buff, ADCBUFFPOS_SHUNTU_RISE,	hadc1.DMA_Handle->Instance->NDTR);
		mcrt.adc_shunt_u_rise = mc_adc_avg(&adc_1_buff);
		mc_adc_CircBuffDemultiplex(&adc_1_buff, ADCBUFFPOS_SHUNTU_FALL,	hadc1.DMA_Handle->Instance->NDTR);
		mcrt.adc_shunt_u_fall = mc_adc_avg(&adc_1_buff);
		mc_adc_CircBuffDemultiplex(&adc_1_buff, ADCBUFFPOS_BUSVOLT_U,	hadc1.DMA_Handle->Instance->NDTR);
		mcrt.adc_phase_u_bus = mc_adc_avg(&adc_1_buff);
		mc_adc_CircBuffDemultiplex(&adc_1_buff, ADCBUFFPOS_EMK_U,		hadc1.DMA_Handle->Instance->NDTR);
		mcrt.adc_phase_u_emk = mc_adc_avg(&adc_1_buff);
		mc_shunt_si(&drv.csa_u, &mcrt.MotCurrRiseSi, mcrt.adc_shunt_u_rise);
		mc_shunt_si(&drv.vdiv_u, &mcrt.MotVoltBusSi, mcrt.adc_phase_u_bus);
	    break;

	    case ccw_pwm:
		mc_adc_CircBuffDemultiplex(&adc_2_buff, ADCBUFFPOS_SHUNTV_RISE,	hadc2.DMA_Handle->Instance->NDTR);
		mcrt.adc_shunt_v_rise = mc_adc_avg(&adc_2_buff);
		mc_adc_CircBuffDemultiplex(&adc_2_buff, ADCBUFFPOS_SHUNTV_FALL,	hadc2.DMA_Handle->Instance->NDTR);
		mcrt.adc_shunt_v_fall = mc_adc_avg(&adc_2_buff);
		mc_adc_CircBuffDemultiplex(&adc_2_buff, ADCBUFFPOS_BUSVOLT_V,	hadc2.DMA_Handle->Instance->NDTR);
		mcrt.adc_phase_v_bus = mc_adc_avg(&adc_2_buff);
		mc_adc_CircBuffDemultiplex(&adc_2_buff, ADCBUFFPOS_EMK_V,	hadc2.DMA_Handle->Instance->NDTR);
		mcrt.adc_phase_v_emk = mc_adc_avg(&adc_2_buff);

		mc_shunt_si(&drv.csa_v, &mcrt.MotCurrRiseSi, mcrt.adc_shunt_v_rise);
		mc_shunt_si(&drv.vdiv_v, &mcrt.MotVoltBusSi, mcrt.adc_phase_v_bus);

	    break;
	    }


	mcbench.rampduty->timestep = timestep_si(&mctime);
	mc_ramp(mcbench.rampduty);
	mcbench.pwm->duty = rampduty.Setpoint;
	pwm.direction = mc_pwm_bcd_update(mcbench.pwm);
	}
	HAL_GPIO_TogglePin(test_GPIO_Port, test_Pin);
	taskdoneflag = 0;
    }

void McTaskInit()
    {


    mcbench.benchsetup = bb_hm7_blower;
    mcbench.pwm = &pwm;
    mcbench.rampduty = &rampduty;
    mcbench.drv = &drv;

    mcbench.adcbuff = &adc_1_buff;

    cmd_init_callbacks(&newcmd);
    confgen_setdefaults(&mcbench);
    drv_en_drv(1);
    drv_setPwmMode(&drv);
    drv_setShuntSign(&drv);
    drv_setShuntGain(&drv.csa_u);

    mc_adc_newBuffer(&adc_1_buff);
    mc_adc_newBuffer(&adc_2_buff);

    HAL_ADC_Start_DMA(&hadc1, (uint32_t*) adc_1_buff.workbuff,	    adc_1_buff.sampleNbr);
    HAL_ADC_Start_DMA(&hadc2, (uint32_t*) adc_2_buff.workbuff,	    adc_2_buff.sampleNbr);
    pwm_init_timer_mc(&pwm);	//stm32 hal init

        while (!dmadoneflag)
	{
	mcbench.pwm->duty = 0.0001;
	mc_pwm_bcd_update(mcbench.pwm);

	osDelay(1000);
	mc_adc_CircBuffDemultiplex(&adc_1_buff, ADCBUFFPOS_SHUNTU_RISE,		hadc1.DMA_Handle->Instance->NDTR);
	drv.csa_u.rawoffset = mc_adc_avg(&adc_1_buff);

	mc_adc_CircBuffDemultiplex(&adc_1_buff, ADCBUFFPOS_BUSVOLT_U,		hadc1.DMA_Handle->Instance->NDTR);
	drv.vdiv_u.rawoffset = mc_adc_avg(&adc_1_buff);

	mc_adc_CircBuffDemultiplex(&adc_2_buff, ADCBUFFPOS_SHUNTV_RISE,		hadc2.DMA_Handle->Instance->NDTR);
	drv.csa_v.rawoffset = mc_adc_avg(&adc_2_buff);

	mc_adc_CircBuffDemultiplex(&adc_2_buff, ADCBUFFPOS_BUSVOLT_V,		hadc2.DMA_Handle->Instance->NDTR);
	drv.vdiv_v.rawoffset = mc_adc_avg(&adc_2_buff);

	}

    initdoneflag = 1;
    }

void StartMcTask(void *argument)
    {
    if(!initdoneflag)
	{
	McTaskInit();
	}

    while (1)
	{
	osDelay(1);
	//if (xSemaphoreTakeFromISR(myFlagNewDMAHandle,0xffff) == pdPASS)
	}


    }

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
    {
    dmadoneflag = 1;
    if (!taskdoneflag)
	McTask();
   // xSemaphoreGiveFromISR(myFlagNewDMAHandle, 0);

    }
