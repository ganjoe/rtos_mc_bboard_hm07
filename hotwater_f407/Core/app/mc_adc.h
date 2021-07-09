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
    EN_RISE,
    EN_FALL,
}
    EN_MC_ADC_CURREDGE;

typedef struct
        {
    /**
     * @note die auswertetiefe ist dynamisch zur laufzeit
     */
        uint32_t filterdepth;
        uint16_t* workbuff;
        /* für bidirektionale messungen */
        uint32_t channels;

        }
    TD_MC_ADC_BUFF;



void mc_shunt_si(TD_MC_DRV_CSA *shunt, float* result, uint32_t raw);

void mc_adc_newBuffer(TD_MC_ADC_BUFF *buff, uint8_t resultscount);

uint32_t mc_adc_avg(TD_MC_ADC_BUFF *buff, uint32_t pos);

extern TD_MC_ADC_BUFF adcbuff;

#endif /* APP_MC_ADC_H_ */
