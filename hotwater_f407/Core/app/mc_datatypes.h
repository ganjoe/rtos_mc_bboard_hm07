/*
 * mc_datatypes.h
 *
 *  Created on: 18.02.2021
 *      Author: pymd02
 *
 *  user api für mc funktionen
 *
 */

#ifndef INC_MC_DATATYPES_H_
#define INC_MC_DATATYPES_H_

#include "main.h"
#include "datatypes.h"
#include "mc_ramp.h"
#include "mc_pwm_if.h"
#include "mc_drv83.h"
#include "mc_adc.h"
#include "newCmdOrder.h"

typedef enum
{

    bb_hm7_blower,
    bb_boardled,
}
	EN_MC_WORKBENCH;

typedef struct
    {
    uint32_t MotCurrRiseRaw, MotCurrFallRaw;	// rohwerte, nach averaging und oversampling
    float shuntrise, shuntfall;	// normierte werte -1 bis 1
    float MotCurrRiseSi,MotCurrFallSi;// verrechnet mit ILSB
    }
    TD_MC_LIVE;

typedef struct
    {
    uint32_t		signature;
    TD_MC_PWM_PARAMS*	pwm;
    EN_MC_WORKBENCH	benchsetup;
    TD_RAMP* 	 	ramp;
    TD_MC_LIVE*		mcrt;
    TD_CMD*		cmd;
    TD_MC_ADC_BUFF*	adcbuff;
    TD_DRV83*		drv;
    TD_MODFLAG*		mf_systick;
    }
	TD_MC_PARAMS;

	void mc_init_BlowerPwm(TD_MC_PWM_PARAMS *pwm);
	void mc_init_BlowerRamp(TD_RAMP* ramp);


extern TD_MC_LIVE mcrt;
extern TD_MC_PARAMS mcbench;


#endif /* INC_MC_DATATYPES_H_ */
