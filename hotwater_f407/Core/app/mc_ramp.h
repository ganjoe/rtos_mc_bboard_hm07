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
	float    TargetValue; 	// Input: Target input (pu)
	float    SetpointValue;	// Output: Target output (pu)	benennung ist texas motorware -style
	float	 nextInc;		// nächstes y für letztes x (pu)
	float    RampStepLimit;	// Limiter für nextInc (praktisch eine mindestrampe um arge pwm sprünge zu vermeiden
	float 	 RampGain;			// Bezug für pu, verhältnis zwischen timestep und nextInc
	float 	 RampTimestep; // ms zeitschritt kann dynamisch sein, in timerticks


	float	 delta;			// diff ein und ausgang
	float	 minInc;			//minimale abweichung für berechnung, bzw skip wenn fast gleich um strom zu sparen


	} RMPCNTL;




void mc_ramp		(RMPCNTL* ramp);
void mc_ramp_init	(RMPCNTL* ramp);



#endif /* INC_MC_RAMP_H_ */
