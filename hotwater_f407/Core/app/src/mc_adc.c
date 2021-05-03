/*
 * mc_adc.c
 *
 *  Created on: Apr 24, 2021
 *      Author: danie
 */


#include "../mc_adc.h"



extern void *pvPortMalloc( size_t xWantedSize );
extern void vPortFree( void *pv );

void mc_adc_newBuffer(TD_MC_ADC_BUFF *buff, uint8_t size)
    {
    buff->filterdepth = size;
    vPortFree(buff->workbuff);

    buff->workbuff = (uint16_t*) pvPortMalloc( buff->filterdepth * sizeof(uint16_t));
    }

float mc_adc_si(TD_MC_ADC_BUFF *buff,TD_MC_ADC_MATH *lsb, uint32_t pos)
    {

    }
float mc_adc_pu(TD_MC_ADC_BUFF *buff,TD_MC_ADC_MATH *lsb, uint32_t pos)
    {

    }

uint32_t mc_adc_avg(TD_MC_ADC_BUFF *buff, uint32_t pos, uint32_t channels)
    {
    /* buffer muss initialisiert sein */
   /* https://www.cypress.com/file/236481/download */
    uint64_t sum = 0;

    for (int var = pos; var <= buff->filterdepth; var+=channels)
	{
	sum += (uint16_t)buff->workbuff[var];
	}

    sum /= (buff->filterdepth/channels);

    }

TD_MC_ADC_BUFF adcbuff;
