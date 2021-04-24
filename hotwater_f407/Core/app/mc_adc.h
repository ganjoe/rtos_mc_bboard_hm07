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

typedef struct
{
    /* raw adc limit	*/
    uint32_t lowlimit, upperlimit;

    /*	pu berechnung	*/
    float max, min;

    /*	si berechnung	*/

    double Ilsb[5];
}
    TD_MC_ADC_MATH;

typedef struct
        {
    /**
     * @note die auswertetiefe ist dynamisch zur laufzeit
     */
        uint32_t filterdepth;
        uint32_t* workbuff;
        uint32_t AdcDmaBuffSize;
        }
    TD_MC_ADC_BUFF;


void mc_adc_newBuffer(TD_MC_ADC_BUFF *buff, uint8_t resultscount);

void mc_adc_avg_alt(TD_MC_ADC_BUFF *buff, uint32_t* sum0, uint32_t* sum1);

extern TD_MC_ADC_BUFF adcbuff;

#endif /* APP_MC_ADC_H_ */
