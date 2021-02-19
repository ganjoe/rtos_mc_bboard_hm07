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
	uint32_t RampDelayMax;	// Parameter: Maximum delay rate (Q0) - independently with global Q
	float    RampLowLimit;	// Parameter: Minimum limit (pu)
	float    RampHighLimit;	// Parameter: Maximum limit (pu)
	uint32_t RampDelayCount; // Variable: Incremental delay (Q0) - independently with global Q
	float    SetpointValue;	// Output: Target output (pu)
	uint32_t EqualFlag;		// Output: Flag output (Q0) - independently with global Q
	float	Tmp;			// Variable: Temp variable
	float		minInc;			//ersatz def in iqmath

	} RMPCNTL;


#define RMPCNTL_DEFAULTS {  0,5,float(-1),float(1),0,0,0,0,0.0001,}


void mc_ramp(RMPCNTL ramp);


#endif /* INC_MC_RAMP_H_ */
