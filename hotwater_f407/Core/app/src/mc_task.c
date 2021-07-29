/*
 * mc_task.c
 *
 *  Created on: Feb 19, 2021
 *      Author: danie
 */

/* Includes für FreeRtos - Tasks und so */


#include "../mc_config.h"



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

    mc_adc_newBuffer(&adcbuff);	// array muss in heap für task-kontext passen

    drv.csa_shunt.rawoffset = drv_adc_ref();

    pwm_init_timer_mc(&pwm);	//stm32 hal init

    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adcbuff.workbuff, adcbuff.buffersize);


    while (1)
	{

	}

    }

void McTask()
    {
    mc_timediff(&mf_systick);

    mcrt.MotCurrRiseRaw = mc_adc_avg(&adcbuff, EN_RISE);
    mcrt.MotCurrFallRaw = mc_adc_avg(&adcbuff, EN_FALL);

    mc_shunt_si(&drv.csa_shunt, &mcrt.MotCurrRiseSi , mcrt.MotCurrRiseRaw);
    mc_shunt_si(&drv.csa_shunt, &mcrt.MotCurrFallSi , mcrt.MotCurrFallRaw);

    mcbench.ramp->timestep = mf_systick.timestep;
    mc_ramp(mcbench.ramp);
    mcbench.pwm->duty = rampe.Setpoint;
    mc_pwm_bcd_update(mcbench.pwm);

    }

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
    {
    McTask();
    HAL_GPIO_TogglePin(test_GPIO_Port, test_Pin);
    HAL_ADC_Start_DMA(&hadc1, adcbuff.workbuff, adcbuff.buffersize);
    }
