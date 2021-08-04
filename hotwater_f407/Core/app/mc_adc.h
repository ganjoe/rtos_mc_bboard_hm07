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
    ADCBUFFPOS_SHUNTV_RISE,
    ADCBUFFPOS_BUSVOLT_U,
    ADCBUFFPOS_BUSVOLT_V,
    ADCBUFFPOS_SHUNTU_FALL,
    ADCBUFFPOS_SHUNTV_FALL,
    ADCBUFFPOS_EMK_U,
    ADCBUFFPOS_EMK_V,
}
    EN_ADC_BCD_BUFFPOS;


typedef struct
        {

        uint16_t buffersize;
        uint16_t filterdepht;
        uint16_t* workbuff;

        uint16_t channels;

        }
    TD_MC_ADC_BUFF;



void mc_shunt_si(TD_MC_DRV_CSA *shunt, float* result, uint32_t raw);

void mc_adc_newBuffer(TD_MC_ADC_BUFF *buff);

uint32_t mc_adc_avg(TD_MC_ADC_BUFF *buff, uint32_t pos);

extern TD_MC_ADC_BUFF adcbuff;

#endif /* APP_MC_ADC_H_ */
