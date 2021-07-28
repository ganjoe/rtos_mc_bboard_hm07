/*
 * mc_adc.c
 *
 *  Created on: Apr 24, 2021
 *      Author: danie
 */

#include "../mc_adc.h"
#include "../utils.h"
#include "../mc_drv83.h"
#include "adc.h"


/* freetos  */
extern void *pvPortMalloc( size_t xWantedSize );

extern void vPortFree( void *pv );

/* für ein hübscheres interface */

void mc_adc_newBuffer(TD_MC_ADC_BUFF *buff, uint8_t size)
    {
    buff->filterdepth = size * buff->channels;
    vPortFree(buff->workbuff);
    buff->workbuff = (uint16_t*) pvPortMalloc( buff->filterdepth * sizeof(uint16_t));
    }

void mc_shunt_si(TD_MC_DRV_CSA *shunt, float* result, uint32_t raw)
    {
    float val;
    val = (float)raw - shunt->rawoffset;

    *result = shunt->lsb * val;
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
