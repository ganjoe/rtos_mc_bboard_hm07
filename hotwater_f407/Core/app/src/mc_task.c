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
	mc_adc_newBuffer(&adcbuff, 64);
	adcbuff.filterdepth = 32;	//filter bezieht sich auf pwm-zyklen
	HAL_ADC_Start_DMA(&hadc1, adcbuff.workbuff, 64);


    /**
     * @brief Setup für dimmbare Melde-Led
     */
    pwm_init_bboard_led1(&pwm_led1);
    mc_init_boardLedPwm(&pwm_led1);
    mc_init_boardLedRamp(&rampe_led1);

    pwm_led1.freq = 1000;
    rampe_led1.Target = 0.5;
    rampe_led1.gain = 1;

/*
    pwm_init_Blower(&pwm);
    mc_init_BlowerPwm(&pwm);


    mcbench.pwm = &pwm;

    mcbench.ramp = &rampe;

    mcbench.pwm->freq = 1000;

    mcbench.ramp->Target = 0.1;

    mcbench.ramp->gain = 0.4;
*/
    while (1)
	{
	osDelay(1);

	mc_timediff(&mf_systick);

	   	/*	adc nach si und pu auswerten	*/
	    uint32_t adcrise, adcfall;
	    mc_adc_avg_alt(&adcbuff, &adcrise, &adcfall);


	    	/*	animierte meldeleuchte	*/
	    rampe_led1.timestep = mf_systick.timestep;
	    mc_ramp(&rampe_led1);
	    pwm_led1.duty = rampe_led1.Setpoint;
	    mc_pwm_update(&pwm_led1);


	}

    }

