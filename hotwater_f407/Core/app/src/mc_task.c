/*
 * mc_task.c
 *
 *  Created on: Feb 19, 2021
 *      Author: danie
 */

/* Includes für FreeRtos - Tasks und so */

#include "../mc_datatypes.h"
#include "../mc_task.h"
#include "../mc_pwm_if.h"
#include "../mc_ramp.h"
#include "../newCmdOrder.h"
#include "../mc_adc.h"
#include "../mc_drv83.h"
#include "../terminal.h"

void StartMcTask(void *argument)
    {
    /**
     * @brief init zeitmessung und terminal
     */
    modflag_init();
    cmd_init_callbacks(&newcmd);

    /**
     * @brief Setup für Motorsitzung
     */
    mcbench.benchsetup = bb_hm7_blower;
    mcbench.pwm = &pwm;		//zuordnung legt fest worauf sich das terminal bezieht
    mcbench.ramp = &rampe;


	//filter bezieht sich auf pwm-zyklen

/* parameter für shunt und spannungsteiler */
    drv.csa_shunt.Ilsb[drv_sgain_40] = 0.00290258;
    drv.csa_shunt.Ilsb[drv_sgain_20] = 0.001438686;
    drv.csa_shunt.Ilsb[drv_sgain_10] = 0.002877372;
    drv.csa_shunt.Ilsb[drv_sgain_5] =  0.005754743;
    drv.csa_shunt.rawoffset = drv_adc_ref();

/* drv83 init */

    drv_en_drv(1);

    drv.modeSelect = drv_pwm_6x;
	drv_setPwmMode(&drv);
    drv.opref = drv_shunt_bidirectinal;
	drv_setShuntSign(&drv);
    drv.csa_shunt.csa_gain = drv_sgain_5;
	drv_setShuntGain(&drv);


    drv.csa_shunt.min = -10;	//bereich in SI für berechnung von pu
    drv.csa_shunt.max = 10;
    drv.csa_shunt.thresh = 10;	//schwelle für wert>0

    drv.emk.Ilsb[drv_sgain_40] = 0.0001;
    drv.emk.min = 0;
    drv.emk.max = 10;
    drv.emk.thresh = 10;


/* pwm timer init */
    pwm_init_timer_mc(&pwm);	//stm32 hal init
    mc_init_BlowerPwm(&pwm);	//mc init
    mc_init_BlowerRamp(&rampe);

/* referenzspannung */

    pwm.htim.Instance->CCR2 = 0x0;
    pwm.htim.Instance->CCR1 = 0x0;
    pwm.htim.Instance->CCR3 = 0x0;
    drv.csa_shunt.rawoffset = drv_adc_ref();

/* startwerte */
    pwm.freq = 40000;
    rampe.Target = -0.1;
    rampe.gain = 0.1;

/* shunt adc mit dyn. buffer starten */
    adcbuff.channels = 2;
    mc_adc_newBuffer(&adcbuff, 10);
    pwm.htim.Instance->CNT = 0;
    HAL_ADC_Start_DMA(&hadc1, adcbuff.workbuff, adcbuff.filterdepth);


    while (1)
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

    }

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
    {
	HAL_GPIO_TogglePin(test_GPIO_Port, test_Pin);
	HAL_ADC_Start_DMA(&hadc1, adcbuff.workbuff, adcbuff.filterdepth);

    }
