/*
 * mc_config.c
 *
 *  Created on: Jul 27, 2021
 *      Author: pymd02
 */
#include "../mc_config.h"


int confgen_setdefaults(TD_MC_PARAMS *mc_params,EN_MC_WORKBENCH setup)
    {
    // init zeitmessung
    modflag_init();
    // init terminalbefehle
    cmd_init_callbacks(&newcmd);

    // datentyp für übersichtliche debugansicht und auto-vervollständigen
    mcbench.benchsetup = bb_hm7_blower;
    mcbench.pwm = &pwm;
    mcbench.ramp = &rampe;
    mcbench.drv = &drv;
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
    }
void initMcTask()
    {
    // init zeitmessung
    modflag_init();
    // init terminalbefehle
    cmd_init_callbacks(&newcmd);

    // datentyp für übersichtliche debugansicht und auto-vervollständigen
    mcbench.benchsetup = bb_hm7_blower;
    mcbench.pwm = &pwm;
    mcbench.ramp = &rampe;
    mcbench.drv = &drv;
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

    }
