/*
 * mc_task.c
 *
 *  Created on: Feb 19, 2021
 *      Author: danie
 */

/* Includes für FreeRtos - Tasks und so */


#include "../mc_config.h"
#include "fatfs.h"
#include "tim.h"

int dmadoneflag=0;
void McTask();
void McTaskInit();

void StartMcTask(void *argument)
    {
    mcbench.benchsetup = bb_hm7_blower;
      mcbench.pwm = &pwm;
      mcbench.ramp = &rampe;
      mcbench.drv = &drv;
      mcbench.mf_systick = &mf_systick;
      mcbench.adcbuff = &adc_1_buff;

     cmd_init_callbacks(&newcmd);
     confgen_setdefaults(&mcbench);
     drv_en_drv(1);
     drv_setPwmMode(&drv);
     drv_setShuntSign(&drv);
     drv_setShuntGain(&drv);

     mc_adc_newBuffer(&adc_1_buff);
     mc_adc_newBuffer(&adc_2_buff);

     drv.csa_shunt.rawoffset = drv_adc_ref();

     //pwm_init_timer_mc(&pwm);	//stm32 hal init

     HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_1_buff.workbuff, adc_1_buff.workbuffsize);
     HAL_ADC_Start_DMA(&hadc2, (uint32_t*)adc_2_buff.workbuff, adc_2_buff.workbuffsize);

     pwm_init_timer_mc(&pwm);	//stm32 hal init

    while (1)
	{

	switch (pwm.direction)
	    {
	    case cw_pwm:
		mcrt.adc_shunt_u_rise = mc_adc_avg(&adc_1_buff, ADCBUFFPOS_SHUNTU_RISE);
		mcrt.adc_shunt_u_fall = mc_adc_avg(&adc_1_buff, ADCBUFFPOS_SHUNTU_FALL);
		mcrt.adc_phase_u_bus = mc_adc_avg(&adc_1_buff, ADCBUFFPOS_BUSVOLT_U);
		mcrt.adc_phase_u_emk = mc_adc_avg(&adc_1_buff, ADCBUFFPOS_EMK_U);
		mc_shunt_si(&drv.csa_shunt, &mcrt.MotCurrRiseSi , mcrt.adc_shunt_u_rise);
		mc_shunt_si(&drv.div_phase, &mcrt.MotVoltBusSi , mcrt.adc_phase_u_bus);
		break;

	    case ccw_pwm:
		mcrt.adc_shunt_v_rise = mc_adc_avg(&adc_2_buff, ADCBUFFPOS_SHUNTV_RISE);
		mcrt.adc_shunt_v_fall = mc_adc_avg(&adc_2_buff, ADCBUFFPOS_SHUNTV_FALL);
		mcrt.adc_phase_v_bus = mc_adc_avg(&adc_2_buff, ADCBUFFPOS_BUSVOLT_V);
		mcrt.adc_phase_v_emk = mc_adc_avg(&adc_2_buff, ADCBUFFPOS_EMK_V);
		mc_shunt_si(&drv.csa_shunt, &mcrt.MotCurrRiseSi , mcrt.adc_shunt_v_rise);
		mc_shunt_si(&drv.div_phase, &mcrt.MotVoltBusSi , mcrt.adc_phase_v_bus);
		break;

		}
		mc_timediff(&mf_systick);
		mcbench.ramp->timestep = mf_systick.timestep;
		mc_ramp(mcbench.ramp);
		mcbench.pwm->duty = rampe.Setpoint;
		pwm.direction = mc_pwm_bcd_update(mcbench.pwm);
	}

    }

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
    {
    dmadoneflag = 1;
     HAL_GPIO_TogglePin(test_GPIO_Port, test_Pin);
    }
