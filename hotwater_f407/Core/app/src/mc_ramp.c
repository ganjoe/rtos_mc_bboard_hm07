/*
 * mc_ramp.c
 *
 *  Created on: 19.02.2021
 *      Author: danie
 */

#include "../mc_ramp.h"
#include "../utils.h"
#include "../mc_datatypes.h"

void mc_ramp(RMPCNTL* ramp)
{
	/* TargetValue ist bei Ti der EINGANG, Setpoint der AUSGANG
	 * Normalerweise meint beides das gleiche, aber wenn mehrere
	 * blöcke hintereinander hängen, braucht man zwei namen 8) */
	ramp->delta = ramp->TargetValue - ramp->SetpointValue;

	/* mindestdifferenz, damit bei ausgleich cpu - zeit gespart wird */
	if (fabs(ramp->delta) > ramp->minInc)
		{

		/* RampGain stellt den Bezug zur Zeit her,
	 	 * für timestep wird die letzte systick - periode eingesetzt */
		ramp->nextInc =  ramp->RampTimestep / ramp->RampGain;

		utils_truncate_number(&ramp->nextInc, 0, ramp->RampStepLimit);

		if(ramp->TargetValue >= ramp->SetpointValue)
			{
			ramp->SetpointValue += ramp->nextInc;
			}
		else
			{
			ramp->SetpointValue -= ramp->nextInc;
			}
		}
}


void mc_ramp_init	(RMPCNTL* ramp)
{
	ramp->minInc = 0.001;
	ramp->RampStepLimit = 0.1;
	ramp->RampGain = 1;

}
