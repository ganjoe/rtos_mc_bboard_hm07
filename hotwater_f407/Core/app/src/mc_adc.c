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
    vPortFree(buff->workbuff);
    buff->AdcDmaBuffSize = size;
    buff->workbuff = (uint32_t*) pvPortMalloc( buff->AdcDmaBuffSize * sizeof(uint32_t));
    }

void mc_adc_avg_alt(TD_MC_ADC_BUFF *buff, uint32_t* sum0, uint32_t* sum1)
    {
    uint32_t sum_rise = 0;
    uint32_t sum_fall = 0;

    for (int var = 0; var < buff->filterdepth; var+=2)
	{
	sum_rise += (uint16_t)buff->workbuff[var];
	sum_fall += (uint16_t)buff->workbuff[var+1];
	}
     sum_rise /= buff->AdcDmaBuffSize;
     sum_fall /= buff->AdcDmaBuffSize;
    }

TD_MC_ADC_BUFF adcbuff;
