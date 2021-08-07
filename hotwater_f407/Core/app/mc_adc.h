/*
 * mc_adc.h
 *
 *  Created on: Apr 24, 2021
 *      Author: danie
 */

#ifndef APP_MC_ADC_H_
#define APP_MC_ADC_H_

#include "main.h"
#include "adc.h"
#include "mc_drv83.h"


typedef enum
    {
    ADCBUFFPOS_SHUNTU_RISE,
    ADCBUFFPOS_BUSVOLT_U,
    ADCBUFFPOS_SHUNTU_FALL,
    ADCBUFFPOS_EMK_U,
    }
    EN_ADC_BCD_BUFFPOS_CW;

    typedef enum
	{
        ADCBUFFPOS_SHUNTV_RISE,
        ADCBUFFPOS_BUSVOLT_V,
        ADCBUFFPOS_SHUNTV_FALL,
        ADCBUFFPOS_EMK_V,
	}
        EN_ADC_BCD_BUFFPOS_CCW;

typedef struct
        {
	uint16_t channels;	//anzahl adc-channels pro sequenz
        uint16_t workbuffsize;	// sequenzen * channel pro sequenz
        uint32_t sequencecount;	// anzahl sequencen
        uint16_t* workbuff;	//ziel für dma-transfer
        uint16_t filterdepht;	//anzahl sequencen a<us denen ein channel gedemuliplext wird
        uint16_t* filterbuff;	// demultiplexte werte aus workbuff
        }
    TD_MC_ADC_BUFF;


void mc_shunt_si(TD_MC_DRV_CSA *shunt, float* result, uint32_t raw);

void mc_adc_newBuffer(TD_MC_ADC_BUFF *buff);

uint32_t mc_adc_avg(TD_MC_ADC_BUFF *buff, uint32_t pos);

/* adc-channel aus dma-buffer demultiplexen,
 * während der puffer geschrieben wird.
 * ringbuffer enthält mehrere sample-sequencen hintereinander.
 *  [chan1, chan2, chan3][chan1, chan2, chan3][chan1, chan2, chan3]
 *
 */
uint32_t mc_adc_CircBuffDemultiplex
				    (TD_MC_ADC_BUFF *adcbuff,
				    uint32_t seqpos, //channelnummer innerhalb einer sequenz
				    uint32_t dmapos //position des schreibcounters
				    );

extern TD_MC_ADC_BUFF adc_1_buff, adc_2_buff;

#endif /* APP_MC_ADC_H_ */
