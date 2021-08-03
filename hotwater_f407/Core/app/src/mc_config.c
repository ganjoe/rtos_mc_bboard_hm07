/*
 * mc_config.c
 *
 *  Created on: Jul 27, 2021
 *      Author: pymd02
 */
#include "../mc_config.h"
#include "../utils_buffer.h"



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

   // drv.csa_shunt.min = -10;	//bereich in SI für berechnung von pu
   // drv.csa_shunt.max = 10;
    drv.csa_shunt.thresh = 10;	//schwelle für wert>0

    drv.emk.Ilsb[drv_sgain_40] = 0.0001;
   // drv.emk.min = 0;
   // drv.emk.max = 10;
    drv.emk.thresh = 10;


/* pwm timer init */

    pwm.speed = 168000000;
    pwm.bits = 0xFFFF;
    pwm.freq = 40000;

    rampe.gain = 1;
    rampe.highlimit = 1;
    rampe.lowlimit = -1.0;
    rampe.timestep = 0.001;
    rampe.RampStepLimit = 0.01;

    adcbuff.channels = 2;
    adcbuff.filterdepht = 10;

    return 1;
    }

int confgen_demultiplex_mcparams(TD_MC_PARAMS *mc, uint8_t* buffer)
    {
	int32_t ind = 0;
	uint32_t signature = buffer_get_uint32(buffer, &ind);

	if (signature != MCPARAMS_SIGNATURE)
	    {
	    return -1;
	    }
        mc->pwm->bits = buffer_get_uint32(buffer, &ind);
	mc->pwm->speed = buffer_get_uint32(buffer, &ind);
	mc->pwm->freq = buffer_get_float16(buffer, 10, &ind);
	mc->pwm->prescaler = buffer_get_uint32(buffer, &ind);
        mc->pwm->top = buffer_get_uint32(buffer, &ind);
        mc->pwm->duty = buffer_get_float16(buffer, 1000, &ind);

	mc->ramp->gain =  buffer_get_float16(buffer, 1000, &ind);
	mc->ramp->highlimit= buffer_get_float16(buffer, 1000, &ind);
	mc->ramp->lowlimit= buffer_get_float16(buffer, 1000, &ind);
	mc->ramp->timestep= buffer_get_float16(buffer, 1000, &ind);
        mc->ramp->RampStepLimit= buffer_get_float16(buffer, 1000, &ind);

        mc->drv->csa_shunt.Ilsb[0] = buffer_get_float32_auto(buffer, &ind);
        mc->drv->csa_shunt.Ilsb[1] = buffer_get_float32_auto(buffer, &ind);
        mc->drv->csa_shunt.Ilsb[2] = buffer_get_float32_auto(buffer, &ind);
        mc->drv->csa_shunt.Ilsb[3] = buffer_get_float32_auto(buffer, &ind);

        mc->drv->csa_shunt.rawoffset = 	buffer_get_uint32(buffer, &ind);
        mc->drv->csa_shunt.thresh = 	buffer_get_uint32(buffer, &ind);

        mc->drv->OLshuntvolts =  	buffer[ind++];
        mc->drv->modeSelect =  		buffer[ind++];
        mc->drv->opref =  		buffer[ind++];
        mc->drv->regAdress =  		buffer[ind++];
        mc->drv->csa_shunt.csa_gain =  	buffer[ind++];

        return ind;

    }
int confgen_multiplex_mcparams	(TD_MC_PARAMS *mc, uint8_t* buffer)

    {
    int32_t ind = 0;

    buffer_append_uint32(buffer, MCPARAMS_SIGNATURE, &ind);
    buffer_append_uint32(buffer, mc->pwm->bits, &ind);
    buffer_append_uint32(buffer, mc->pwm->speed, &ind);
    buffer_append_float16(buffer, mc->pwm->freq,10, &ind);
    buffer_append_uint32(buffer, mc->pwm->prescaler, &ind);
    buffer_append_uint32(buffer, mc->pwm->top, &ind);
    buffer_append_float16(buffer, mc->pwm->duty,1000, &ind);

    buffer_append_float16(buffer, mc->ramp->gain,1000, &ind);
    buffer_append_float16(buffer, mc->ramp->highlimit,1000, &ind);
    buffer_append_float16(buffer, mc->ramp->lowlimit,1000, &ind);
    buffer_append_float16(buffer, mc->ramp->timestep,1000, &ind);
    buffer_append_float16(buffer, mc->ramp->RampStepLimit,1000, &ind);

    buffer_append_float32_auto(buffer, mc->drv->csa_shunt.Ilsb[0], &ind);
    buffer_append_float32_auto(buffer, mc->drv->csa_shunt.Ilsb[1], &ind);
    buffer_append_float32_auto(buffer, mc->drv->csa_shunt.Ilsb[2], &ind);
    buffer_append_float32_auto(buffer, mc->drv->csa_shunt.Ilsb[3], &ind);

    buffer_append_uint32(buffer, mc->drv->csa_shunt.rawoffset, &ind);
    buffer_append_uint32(buffer, mc->drv->csa_shunt.thresh, &ind);

    buffer[ind++] = mc->drv->OLshuntvolts;
    buffer[ind++] = mc->drv->modeSelect;
    buffer[ind++] = mc->drv->opref;
    buffer[ind++] = mc->drv->regAdress;
    buffer[ind++] = mc->drv->csa_shunt.csa_gain;

    return ind;
    }

int confgen_storeSD	(char* filename)
    {
    int byteswrote, confsize;
    uint8_t confbuffer[CONFGEN_BUFFERSIZE];
    confsize = confgen_multiplex_mcparams(&mcbench, confbuffer);
    for (int var = confsize; var < CONFGEN_BUFFERSIZE; ++var)
	{
	confbuffer[var] = "@";
	}
    byteswrote = sd_writebuffer(filename, confbuffer, CONFGEN_BUFFERSIZE, 0);
    if (confsize<=byteswrote)
	return confsize;
    else
	return -1;
    }

int confgen_loadSD	(uint8_t* buffer, const char* filename)
    {
    return sd_readbuffer(filename, buffer, CONFGEN_BUFFERSIZE, 0);
    }

void initMcTask()
    {


    }
