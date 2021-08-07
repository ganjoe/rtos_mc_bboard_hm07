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
    buff->workbuffsize = buff->workbuffsize * buff->channels;
    vPortFree(buff->workbuff);
    vPortFree(buff->filterbuff);
    buff->workbuff = (uint16_t*) pvPortMalloc(buff->workbuffsize * sizeof(uint16_t));
    buff->filterbuff = (uint16_t*) pvPortMalloc(buff->filterdepht * sizeof(uint16_t));
    }

void mc_shunt_si(TD_MC_DRV_CSA *shunt, float *result, uint32_t raw)
    {
    float val;
    val = (float) raw - shunt->rawoffset;

    *result = shunt->lsb * val;
    }

uint32_t mc_adc_CircBuffDemultiplex(TD_MC_ADC_BUFF *adcbuff, uint32_t seqpos,uint32_t dmapos)
    {// 4 uhr morgens
    int i = 0;
    int ir = 0;
    int fd = adcbuff->filterdepht;	//resultbufferindex

    i = dmapos / adcbuff->channels;
    i *= adcbuff->channels;
    i += seqpos;

    if (i > dmapos)	{i -= adcbuff->channels;}

    while (fd)
	{
	if (i < 0)   {  i += adcbuff->workbuffsize;   }
	adcbuff->filterbuff[ir] = adcbuff->workbuff[i];
	i -= adcbuff->channels;
	ir++;
	fd--;
	}
    }

uint32_t mc_adc_avg(TD_MC_ADC_BUFF *buff, uint32_t pos)
{
/* buffer muss initialisiert sein */

uint64_t sum = 0;
uint32_t var = 0;

for (var = pos; var <= buff->workbuffsize - 1; var += buff->channels)
    {
    sum += (uint16_t) buff->workbuff[var];
    }

sum /= (buff->workbuffsize / buff->channels);

return sum;

}

TD_MC_ADC_BUFF adc_1_buff, adc_2_buff;
