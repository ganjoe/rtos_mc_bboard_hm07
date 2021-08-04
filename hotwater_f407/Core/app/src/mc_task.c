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

int dmadoneflag=1;
void McTask();
void McTaskInit();

void StartMcTask(void *argument)
    {
    mcbench.benchsetup = bb_hm7_blower;
      mcbench.pwm = &pwm;
      mcbench.ramp = &rampe;
      mcbench.drv = &drv;
      mcbench.mf_systick = &mf_systick;
      mcbench.adcbuff = &adcbuff;

     cmd_init_callbacks(&newcmd);
     confgen_setdefaults(&mcbench);
     drv_en_drv(1);
     drv_setPwmMode(&drv);
     drv_setShuntSign(&drv);
     drv_setShuntGain(&drv);

     mc_adc_newBuffer(&adcbuff);

     drv.csa_shunt.rawoffset = drv_adc_ref();

     pwm_init_timer_mc(&pwm);	//stm32 hal init

     HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adcbuff.workbuff, adcbuff.buffersize);

    while (1)
	{
        if(dmadoneflag)
	    {
	    dmadoneflag=0;
	    mc_timediff(&mf_systick);

	    mcrt.adc_shunt_u_rise = mc_adc_avg(&adcbuff, ADCBUFFPOS_SHUNTU_RISE);
	    mcrt.adc_shunt_u_fall = mc_adc_avg(&adcbuff, ADCBUFFPOS_SHUNTU_FALL);
	    mcrt.adc_shunt_v_rise = mc_adc_avg(&adcbuff, ADCBUFFPOS_SHUNTV_RISE);
	    mcrt.adc_shunt_v_fall = mc_adc_avg(&adcbuff, ADCBUFFPOS_SHUNTV_FALL);
	    mcrt.adc_phase_u_bus = mc_adc_avg(&adcbuff, ADCBUFFPOS_BUSVOLT_U);
	    mcrt.adc_phase_v_bus = mc_adc_avg(&adcbuff, ADCBUFFPOS_BUSVOLT_V);
	    mcrt.adc_phase_u_emk = mc_adc_avg(&adcbuff, ADCBUFFPOS_EMK_U);
	    mcrt.adc_phase_v_emk = mc_adc_avg(&adcbuff, ADCBUFFPOS_EMK_V);


	    mc_shunt_si(&drv.csa_shunt, &mcrt.MotCurrRiseSi , mcrt.adc_shunt_u_rise);
	    mc_shunt_si(&drv.csa_shunt, &mcrt.MotCurrFallSi , mcrt.adc_shunt_u_fall);


	    mcbench.ramp->timestep = mf_systick.timestep;
	    mc_ramp(mcbench.ramp);
	    mcbench.pwm->duty = rampe.Setpoint;
	    mc_pwm_bcd_update(mcbench.pwm);


	    }
	}

    }



void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
    {
    dmadoneflag = 1;

    HAL_GPIO_TogglePin(test_GPIO_Port, test_Pin);

    }
