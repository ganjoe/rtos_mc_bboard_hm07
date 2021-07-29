/*
 * mc_config.c
 *
 *  Created on: Jul 27, 2021
 *      Author: pymd02
 */
#include "../mc_config.h"


int confgen_setdefaults(TD_MC_PARAMS *mc)
    {
    // init zeitmessung
    mf_systick.timerspeed = 168000000;

/* parameter für shunt und spannungsteiler */
    drv.csa_shunt.Ilsb[drv_sgain_40] = 0.00290258;
    drv.csa_shunt.Ilsb[drv_sgain_20] = 0.001438686;
    drv.csa_shunt.Ilsb[drv_sgain_10] = 0.002877372;
    drv.csa_shunt.Ilsb[drv_sgain_5] =  0.005754743;

/* drv83 init */

    drv.modeSelect = drv_pwm_6x;
    drv.opref = drv_shunt_bidirectinal;
    drv.csa_shunt.csa_gain = drv_sgain_5;

    drv.csa_shunt.min = -10;	//bereich in SI für berechnung von pu
    drv.csa_shunt.max = 10;
    drv.csa_shunt.thresh = 10;	//schwelle für wert>0

    drv.emk.Ilsb[drv_sgain_40] = 0.0001;
    drv.emk.min = 0;
    drv.emk.max = 10;
    drv.emk.thresh = 10;


/* pwm timer init */

    pwm.speed = 168000000;
    pwm.bits = 0xFFFF;
    pwm.duty_max = 0.9999;
    pwm.duty_min = 0.00001;
    pwm.freq_max = 64000;
    pwm.freq_min = 0xFF;
    pwm.freq = 40000;

    rampe.gain = 1;
    rampe.highlimit = 1;
    rampe.lowlimit = -1.0;
    rampe.timestep = 0.001;
    rampe.RampStepLimit = 0.01;

    adcbuff.channels = 2;
    adcbuff.filterdepht = 10;


    }
void initMcTask()
    {


    }
