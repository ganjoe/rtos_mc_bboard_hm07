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


    termlog.ticks_update_terminal = 1000;

/* parameter für shunt und spannungsteiler */
    drv.csa_u.Ilsb[drv_sgain_40] = 0.00290258;
    drv.csa_u.Ilsb[drv_sgain_20] = 0.001438686;
    drv.csa_u.Ilsb[drv_sgain_10] = 0.002877372;
    drv.csa_u.Ilsb[drv_sgain_5] =  0.005754743;

    drv.vdiv_u.lsb = 0.0371;
    drv.vdiv_v.lsb = 0.0371;
    drv.vdiv_u.rawoffset = 50;
    drv.vdiv_v.rawoffset = 50;

/* drv83 init */

    drv.modeSelect = drv_pwm_6x;
    drv.opref = drv_shunt_bidirectinal;
    drv.csa_u.rawoffset = 2040;	//sollte bei init überschrieben werden
    drv.csa_v.rawoffset = 2040;

    drv.csa_u.csa_gain = drv_sgain_5;
    drv.csa_v.csa_gain = drv_sgain_5;
    drv.csa_u.thresh = 10;	//schwelle für wert>0



/* pwm timer init */

    pwm.speed = 168000000;
    pwm.bits = 0xFFFF;
    pwm.freq = 40000;

    rampduty.gain = 1;
    rampduty.highlimit = 1;
    rampduty.lowlimit = -1.0;
    rampduty.timestep = 0.001;
    rampduty.RampStepLimit = 0.5;

    adc_1_buff.channels = 4;
    adc_1_buff.sampleNbr = 30;
    adc_1_buff.filterNbr = 30;

    adc_2_buff.channels = 4;
    adc_2_buff.sampleNbr = 30;
    adc_2_buff.filterNbr = 30;


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

	mc->rampduty->gain =  buffer_get_float16(buffer, 1000, &ind);
	mc->rampduty->highlimit= buffer_get_float16(buffer, 1000, &ind);
	mc->rampduty->lowlimit= buffer_get_float16(buffer, 1000, &ind);
	mc->rampduty->timestep= buffer_get_float16(buffer, 1000, &ind);
        mc->rampduty->RampStepLimit= buffer_get_float16(buffer, 1000, &ind);

        mc->drv->csa_u.Ilsb[0] = buffer_get_float32_auto(buffer, &ind);
        mc->drv->csa_u.Ilsb[1] = buffer_get_float32_auto(buffer, &ind);
        mc->drv->csa_u.Ilsb[2] = buffer_get_float32_auto(buffer, &ind);
        mc->drv->csa_u.Ilsb[3] = buffer_get_float32_auto(buffer, &ind);

        mc->drv->csa_u.rawoffset = 	buffer_get_uint32(buffer, &ind);
        mc->drv->csa_u.thresh = 	buffer_get_uint32(buffer, &ind);

        mc->drv->OLshuntvolts =  	buffer[ind++];
        mc->drv->modeSelect =  		buffer[ind++];
        mc->drv->opref =  		buffer[ind++];
        mc->drv->regAdress =  		buffer[ind++];
        mc->drv->csa_u.csa_gain =  	buffer[ind++];

        //12.08.21 nicht getestet
        mc->adcbuff_1->channels  =buffer_get_uint16(buffer, &ind);
        mc->adcbuff_1->sampleNbr =buffer_get_uint16(buffer, &ind);
        mc->adcbuff_1->filterNbr =buffer_get_uint16(buffer, &ind);

        mc->adcbuff_2->channels = buffer_get_uint16(buffer, &ind);
        mc->adcbuff_2->sampleNbr= buffer_get_uint16(buffer, &ind);
        mc->adcbuff_2->filterNbr= buffer_get_uint16(buffer, &ind);




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

    buffer_append_float16(buffer, mc->rampduty->gain,1000, &ind);
    buffer_append_float16(buffer, mc->rampduty->highlimit,1000, &ind);
    buffer_append_float16(buffer, mc->rampduty->lowlimit,1000, &ind);
    buffer_append_float16(buffer, mc->rampduty->timestep,1000, &ind);
    buffer_append_float16(buffer, mc->rampduty->RampStepLimit,1000, &ind);

    buffer_append_float32_auto(buffer, mc->drv->csa_u.Ilsb[0], &ind);
    buffer_append_float32_auto(buffer, mc->drv->csa_u.Ilsb[1], &ind);
    buffer_append_float32_auto(buffer, mc->drv->csa_u.Ilsb[2], &ind);
    buffer_append_float32_auto(buffer, mc->drv->csa_u.Ilsb[3], &ind);

    buffer_append_uint32(buffer, mc->drv->csa_u.rawoffset, &ind);
    buffer_append_uint32(buffer, mc->drv->csa_u.thresh, &ind);

    buffer[ind++] = mc->drv->OLshuntvolts;
    buffer[ind++] = mc->drv->modeSelect;
    buffer[ind++] = mc->drv->opref;
    buffer[ind++] = mc->drv->regAdress;
    buffer[ind++] = mc->drv->csa_u.csa_gain;

    //12.08.21
    buffer_append_uint16(buffer, mc->adcbuff_1->channels, &ind);
    buffer_append_uint16(buffer, mc->adcbuff_1->sampleNbr, &ind);
    buffer_append_uint16(buffer, mc->adcbuff_1->filterNbr, &ind);

    buffer_append_uint16(buffer, mc->adcbuff_2->channels, &ind);
    buffer_append_uint16(buffer, mc->adcbuff_2->sampleNbr, &ind);
    buffer_append_uint16(buffer, mc->adcbuff_2->filterNbr, &ind);

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


