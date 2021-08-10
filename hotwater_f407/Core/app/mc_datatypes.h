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
#include "mc_dlog.h"

typedef enum
{

    bb_hm7_blower,
    bb_boardled,
}
	EN_MC_WORKBENCH;



typedef struct
    {
    uint32_t adc_shunt_u_rise, adc_shunt_u_fall;	// rohwerte, nach averaging und oversampling
    uint32_t adc_shunt_v_rise, adc_shunt_v_fall;
    uint32_t adc_phase_u_bus, adc_phase_v_bus;
    uint32_t adc_phase_u_emk, adc_phase_v_emk;

    float MotCurrRiseSi,MotCurrFallSi;
    float MotVoltBusSi, MotVoltEmkSi;

    }
    TD_MC_LIVE;

typedef struct
    {
    uint32_t		signature;
    TD_MC_PWM_PARAMS*	pwm;
    EN_MC_WORKBENCH	benchsetup;
    TD_RAMP* 	 	rampduty;
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
