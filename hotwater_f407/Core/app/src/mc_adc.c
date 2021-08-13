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

/* thema oversampling: https://www.cypress.com/file/236481/download */

/* freetos  */
extern void* pvPortMalloc(size_t xWantedSize);

extern void vPortFree(void *pv);

/* für ein hübscheres interface */

void mc_adc_newBuffer(TD_MC_ADC_BUFF *buff)
    {
    buff->sampleNbr = buff->sampleNbr * buff->channels;
    vPortFree(buff->workbuff);
    vPortFree(buff->filterbuff);
    buff->workbuff = (uint16_t*) pvPortMalloc(buff->sampleNbr * sizeof(uint16_t));
    buff->filterbuff = (uint16_t*) pvPortMalloc(buff->filterNbr * sizeof(uint16_t));

    }

void mc_adc_restartBuffer(TD_MC_ADC_BUFF *adc_buff)
    {
     //HAL_ADC_Stop_DMA(&adc_1_buff);
    //HAL_ADC_Stop_DMA(&adc_2_buff);
    mc_adc_newBuffer(adc_buff);
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*) adc_buff->workbuff,adc_buff->sampleNbr);
      }

void mc_shunt_si(TD_MC_DRV_CSA *shunt, float *result, uint32_t raw)
    {
    float val;
    val = (float) raw - shunt->rawoffset;

    *result = shunt->lsb * val;
    }

void mc_phase_si(TD_MC_DRV_CSA *shunt, float *result, uint32_t raw)
    {
    float val;
    val = (float) raw;

    *result = shunt->lsb * val;
    }

uint32_t mc_adc_CircBuffDemultiplex(TD_MC_ADC_BUFF *adcbuff, uint32_t seqpos,uint32_t dmapos)
    {// 4 uhr morgens
    //am besten mit exel-tabelle nachvollziehen
    int i = 0;
    int ir = 0;
    int fd = adcbuff->filterNbr;

    i = dmapos / adcbuff->channels;
    i *= adcbuff->channels;
    i += seqpos;

    if (i > dmapos)
	{
	i -= adcbuff->channels;
	}

    while (fd)
	{
	if (i < 0)
	    {
	    i += adcbuff->sampleNbr;
	    }
	adcbuff->filterbuff[ir] = adcbuff->workbuff[i];
	i -= adcbuff->channels;
	ir++;
	fd--;
	}
    }

uint32_t mc_adc_avg(TD_MC_ADC_BUFF *buff)
{
/* buffer muss initialisiert sein */

uint32_t sum = 0;
uint32_t var = 0;

for (; var <= buff->filterNbr; var ++)
    {
    sum += (uint16_t) buff->filterbuff[var];
    }
sum /= buff->filterNbr;

return sum;

}

TD_MC_ADC_BUFF adc_1_buff, adc_2_buff, adc_3_buff;
