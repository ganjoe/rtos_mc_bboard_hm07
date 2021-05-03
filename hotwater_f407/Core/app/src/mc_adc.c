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

void mc_adc_avg_alt(TD_MC_ADC_BUFF *buff, uint32_t* sum0, uint32_t* sum1)
    {
    uint32_t sum_rise = 0;
    uint32_t sum_fall = 0;

    for (int var = 0; var <= buff->filterdepth; var+=2)
	{
	sum_rise += (uint16_t)buff->workbuff[var];
	}
    for (int var = 1; var <= buff->filterdepth; var+=2)
	{
	sum_fall += (uint16_t)buff->workbuff[var];
	}
     sum_rise /= (buff->filterdepth/2);
     sum_fall /= (buff->filterdepth/2);
    }

uint32_t mc_adc_avg(TD_MC_ADC_BUFF *buff, uint32_t* pos)
    {

    }

TD_MC_ADC_BUFF adcbuff;
