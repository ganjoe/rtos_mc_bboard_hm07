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
    /* für bidirektionale messungen */
    uint32_t rawoffset;

    /* raw adc limit	*/
    uint32_t thresh;

    /*	pu berechnung	*/
    float max, min;

    /*	si berechnung	*/
    float Ilsb[5];


}
    TD_MC_ADC_MATH;

typedef enum
{
    current_rise,
    current_fall,
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
        uint32_t rawoffset;
        uint32_t channels;

        	//bzw. referenzspannung bei bidirektionalen shunts etc.

        }
    TD_MC_ADC_BUFF;

int mc_adc_ref(TD_MC_ADC_BUFF *buff);
float mc_adc_si(TD_MC_ADC_MATH *lsb, uint32_t pos, uint32_t offset);
float mc_adc_pu(TD_MC_ADC_MATH *lsb, uint32_t pos, uint32_t offset);

void mc_adc_newBuffer(TD_MC_ADC_BUFF *buff, uint8_t resultscount);
uint32_t mc_adc_avg(TD_MC_ADC_BUFF *buff, uint32_t pos);

extern TD_MC_ADC_BUFF adcbuff;

#endif /* APP_MC_ADC_H_ */
