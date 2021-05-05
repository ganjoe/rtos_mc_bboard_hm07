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


/* drv83 init */
    drv_en_drv(1);
    drv.modeSelect = drv_pwm_6x; 	drv_setPwmMode(&drv);
    drv.csa_gain = drv_sgain_40; 	drv_setShuntGain(&drv);
    drv.opref = drv_shunt_bidirectinal; drv_setShuntSign(&drv);

/* parameter für analog */
    shunt.Ilsb[drv_sgain_40] = 0.000719343;
    shunt.Ilsb[drv_sgain_20] = 0.001438686;
    shunt.Ilsb[drv_sgain_10] = 0.002877372;
    shunt.Ilsb[drv_sgain_5] =  0.005754743;
    shunt.Ilsb[drv_sgain_1] = 0.002877372;


    shunt.min = -10;
    shunt.max = 10;			//bereich in SI für berechnung von pu
    shunt.thresh = 10;


    emk.Ilsb[drv_sgain_40] = 0.0001;
    emk.min = 0;
    emk.max = 10;
    emk.thresh = 10;


/* pwm timer init */
    pwm_init_timer_mc(&pwm);	//stm32 hal init
    mc_init_BlowerPwm(&pwm);	//mc init
    mc_init_BlowerRamp(&rampe);

/* referenzspannung */

    pwm.htim.Instance->CCR2 = 0x0;
    pwm.htim.Instance->CCR1 = 0x0;
    pwm.htim.Instance->CCR3 = 0x0;
    mc_adc_ref(&adcbuff);

/* startwerte */
    pwm.freq = 10000;
    rampe.Target = -0.0;
    rampe.gain = 1;

/* shunt adc mit dyn. buffer starten */
    mc_adc_newBuffer(&adcbuff, 10);
    pwm.htim.Instance->CNT = 0;
    HAL_ADC_Start_DMA(&hadc1, adcbuff.workbuff, adcbuff.filterdepth);

/* led setup */
   // pwm_init_timer_led1(&pwm_led1);
    mc_init_boardLedPwm(&pwm_led1);
    mc_init_boardLedRamp(&rampe_led1);

    pwm_led1.freq = 10000;
    rampe_led1.Target = 0.2;
    rampe_led1.gain = 1;


    while (1)
	{
	osDelay(1);

	mc_timediff(&mf_systick);

	    uint32_t adcrise, adcfall;	// rohwerte, nach averaging und oversampling
	    float shuntrise, shuntfall;	// normierte werte -1 bis 1


	    adcrise = mc_adc_avg(&adcbuff, current_rise, 2);
	    adcfall = mc_adc_avg(&adcbuff, current_fall, 2);

	    shuntrise = mc_adc_pu(&shunt, adcrise, adcbuff.rawoffset);
	    shuntfall = mc_adc_pu(&shunt, adcfall, adcbuff.rawoffset);

	    float sicurrent = mc_adc_si(&shunt, adcrise, adcbuff.rawoffset);

	    mcbench.ramp->timestep = mf_systick.timestep;
	    mc_ramp(mcbench.ramp);
	    mcbench.pwm->duty = rampe.Setpoint;
	    mc_pwm_bcd_update(mcbench.pwm);


	    	/*	animierte meldeleuchte
	    rampe_led1.timestep = mf_systick.timestep;
	    mc_ramp(&rampe_led1);
	    pwm_led1.duty = rampe_led1.Setpoint;
	    mc_pwm_led_update(&pwm_led1);*/
	}

    }

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
    {

	HAL_GPIO_TogglePin(test_GPIO_Port, test_Pin);
	pwm.htim.Instance->CNT = 0;
	HAL_ADC_Start_DMA(&hadc1, adcbuff.workbuff, adcbuff.filterdepth);


    }
