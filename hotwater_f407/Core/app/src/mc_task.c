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


    drv_en_drv(1);
    drv.modeSelect = drv_pwm_6x; 	drv_setPwmMode(&drv);
    drv.csa_gain = drv_sgain_40; 	drv_setShuntGain(&drv);
    drv.opref = drv_shunt_bidirectinal; drv_setShuntSign(&drv);

    shunt.Ilsb[drv_sgain_40] = 0.0001;
    shunt.min = 0;
    shunt.max = 10;			//bereich in SI für berechnung von pu
    shunt.lowlimit = 10;
    shunt.upperlimit = 0xFFF -10;	//limiter für adc-raw

    emk.Ilsb[drv_sgain_40] = 0.0001;
    emk.min = 0;
    emk.max = 10;
    emk.lowlimit = 10;
    emk.upperlimit = 0xFFF -10;

    pwm_init_timer_mc(&pwm);	//stm32 hal init
    mc_init_BlowerPwm(&pwm);	//mc init
    mc_init_BlowerRamp(&rampe);

    pwm.freq = 1000;
    rampe.Target = -0.0;
    rampe.gain = 1;


    /**
     * @brief Setup für dimmbare Melde-Led
     */
    //pwm_init_timer_led1(&pwm_led1);
    mc_init_boardLedPwm(&pwm_led1);
    mc_init_boardLedRamp(&rampe_led1);

    pwm_led1.freq = 10000;
    rampe_led1.Target = 0.2;
    rampe_led1.gain = 1;

    mc_adc_newBuffer(&adcbuff, 10);
    HAL_ADC_Start_DMA(&hadc1, adcbuff.workbuff, adcbuff.filterdepth);

    /**
     * @brief Setup für Regelung
     */


    while (1)
	{
	osDelay(1);

	mc_timediff(&mf_systick);

	   	/*	adc nach si und pu auswerten	*/
	    uint32_t adcrise, adcfall;
	   // mc_adc_avg_alt(&adcbuff, &adcrise, &adcfall);
	    adcrise = mc_adc_avg(&adcbuff, current_rise, 2);
	    adcfall = mc_adc_avg(&adcbuff, current_fall, 2);


	    mcbench.ramp->timestep = mf_systick.timestep;
	    mc_ramp(mcbench.ramp);
	    mcbench.pwm->duty = rampe.Setpoint;
	    mc_pwm_bcd_update(mcbench.pwm);


	    	/*	animierte meldeleuchte	*/
	    rampe_led1.timestep = mf_systick.timestep;
	    mc_ramp(&rampe_led1);
	    pwm_led1.duty = rampe_led1.Setpoint;
	    mc_pwm_led_update(&pwm_led1);
	}

    }

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
    {

    HAL_GPIO_TogglePin(test_GPIO_Port, test_Pin);
    HAL_ADC_Start_DMA(&hadc1, adcbuff.workbuff, adcbuff.filterdepth);

    }
