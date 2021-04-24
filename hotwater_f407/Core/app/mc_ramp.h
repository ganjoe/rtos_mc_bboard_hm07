/*
 * mc_ramp.h
 *
 *  Created on: 17.02.2021
 *      Author: pymd02
 *      fork von texas motorware für iq-math
 */

#ifndef INC_MC_RAMP_H_
#define INC_MC_RAMP_H_

#include "main.h"


typedef struct
	{
	float   Target; 		// Input: Target input (pu)
	float   Setpoint;		// Output: Target output (pu)
	float	 nextInc;		// nächstes y für letztes x (pu)
	float   RampStepLimit;	// Limiter für nextInc (praktisch eine mindestrampe um arge pwm sprünge zu vermeiden
	float 	 gain;			// Bezug für pu, verhältnis zwischen timestep und nextInc
	float	 highlimit, lowlimit;	// Bezug für Gain. die rampe wird flacher, nicht nur geclippt
	float 	 timestep; // ms zeitschritt kann dynamisch sein, in timerticks
	float	 delta;			// diff ein und ausgang
	float	 minInc;			//minimale abweichung für berechnung, bzw skip wenn fast gleich um strom zu sparen

	} RMPCNTL;


void mc_ramp		(RMPCNTL* ramp);

extern RMPCNTL rampe, rampe_led1;

#endif /* INC_MC_RAMP_H_ */

