/*
 * mc_ramp.c
 *
 *  Created on: 19.02.2021
 *      Author: daniel
 */

#include "../mc_ramp.h"



void mc_ramp(TD_RAMP *ramp, TD_TIMESTEP *time)
    {
    /*  Ti - Style: TargetValue ist bei der EINGANG, Setpoint der AUSGANG
     * Normalerweise meint beides das gleiche, aber wenn mehrere
     * blöcke hintereinander hängen, braucht man zwei namen 8) */


	ramp->nextInc =  time->timestep * ramp->gain;

	utils_truncate_number(&ramp->nextInc, 1E-9, ramp->RampStepLimit);
	utils_truncate_number(&ramp->Target, ramp->lowlimit, ramp->highlimit);

	if(ramp->Target >= ramp->Setpoint)
		{
		ramp->Setpoint += ramp->nextInc;
		if (ramp->Setpoint > ramp->Target )
		    ramp->Setpoint = ramp->Target;
		}
	else
		{
		ramp->Setpoint -= ramp->nextInc;
		if (ramp->Setpoint < ramp->Target )
		    ramp->Setpoint = ramp->Target;
		}
    }


TD_RAMP rampduty, rampvolt;


