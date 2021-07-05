/*
 * mc_adc.c
 *
 *  Created on: Apr 24, 2021
 *      Author: danie
 */


#include "../mc_adc.h"
#include "../utils.h"
#include "../mc_drv83.h"


/* freetos  */
extern void *pvPortMalloc( size_t xWantedSize );
extern void vPortFree( void *pv );

/* für ein hübscheres interface */

int mc_adc_ref(TD_MC_ADC_BUFF *buff)
    {
    uint32_t sum = 0;
    uint32_t samples = 1024;
    uint32_t ovrsample = 256;
    uint32_t adcresolution = 0xfff;
    uint32_t maxsamples = 0xffffffff / (ovrsample *adcresolution);
    int did_trunc;
    did_trunc = utils_truncate_number_int32(samples, 1, maxsamples);

     for (int var = 0; var < samples; ++var)
 	{
 	HAL_ADC_Start(&hadc1);
 	if (HAL_ADC_PollForConversion(&hadc1, HAL_TIMEOUT) == HAL_OK)
 	    {
 	   sum += HAL_ADC_GetValue(&hadc1)*ovrsample;
 	    }
 	}
     sum/=samples;
     buff->rawoffset = sum /ovrsample;
     return did_trunc;
    }

void mc_adc_newBuffer(TD_MC_ADC_BUFF *buff, uint8_t size)
    {
    buff->filterdepth = size * buff->channels;
    vPortFree(buff->workbuff);
    buff->workbuff = (uint16_t*) pvPortMalloc( buff->filterdepth * sizeof(uint16_t));
    }

float mc_adc_si(TD_MC_ADC_MATH *lsb, uint32_t pos, uint32_t offset)
    {
    int32_t val = pos - offset;
    return (float)val  * lsb->Ilsb[drv_sgain_40];
    }
float mc_adc_pu(TD_MC_ADC_MATH *lsb, uint32_t pos, uint32_t offset)
    {
    float si = mc_adc_si(lsb, pos, offset);
    utils_map_ptr(&si, lsb->min, lsb->max, -1, 1);
    return si;
    }

uint32_t mc_adc_avg(TD_MC_ADC_BUFF *buff, uint32_t pos)
    {
    /* buffer muss initialisiert sein */
    /* thema oversampling: */
   /* https://www.cypress.com/file/236481/download */
    uint64_t sum = 0;
    uint32_t var = 0;

    for (var = pos; var <= buff->filterdepth -1 ; var += buff->channels)
	{
	sum += (uint16_t)buff->workbuff[var];
	}

    sum /= (buff->filterdepth/buff->channels);

    return sum;

    }

TD_MC_ADC_BUFF adcbuff;
