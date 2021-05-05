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

void mc_adc_ref(TD_MC_ADC_BUFF *buff)
    {
    uint32_t sum = 0;



     for (int var = 0; var < 256; ++var)
 	{
 	HAL_ADC_Start(&hadc1);
 	if (HAL_ADC_PollForConversion(&hadc1, 1000000) == HAL_OK)
 	    {
 	   sum += HAL_ADC_GetValue(&hadc1);
 	    }

 	}
     sum/=256;
     buff->rawoffset = sum;
    }

void mc_adc_newBuffer(TD_MC_ADC_BUFF *buff, uint8_t size)
    {
    buff->filterdepth = size;
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
    return mc_adc_si(lsb, pos, offset);
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

    return sum;

    }

TD_MC_ADC_BUFF adcbuff;
