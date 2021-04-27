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

typedef enum
{
	bb_hm7_blower,
	bb_boardled,
}
	EN_MC_WORKBENCH;

typedef struct
{
	TD_MC_PWM_PARAMS* pwm;
	EN_MC_WORKBENCH	 benchsetup;
	RMPCNTL* 	 ramp;
}
	TD_MC_PARAMS;

void mc_init_default(TD_MC_PARAMS* mcbench);

void mc_init_boardLedPwm(TD_MC_PWM_PARAMS *pwm);
void mc_init_BlowerPwm(TD_MC_PWM_PARAMS *pwm);
void mc_init_boardLedRamp(RMPCNTL* ramp);
void mc_init_BlowerRamp(RMPCNTL* ramp);

extern TD_MC_PARAMS mcbench;

#endif /* INC_MC_DATATYPES_H_ */
