/*
 * mc_ramp.c
 *
 *  Created on: 19.02.2021
 *      Author: danie
 */

#include "../mc_ramp.h"
#include "../utils.h"

void mc_ramp(RMPCNTL ramp)
{
	/* TargetValue ist bei Ti der EINGANG, Setpoint der AUSGANG
	 * Normalerweise meint beides das gleiche, aber wenn mehrere
	 * blöcke hintereinander hängen, braucht man zwei namen 8) */
	ramp.Tmp = ramp.TargetValue - ramp.SetpointValue;

	/* minimale Auflösung des Datentypes oder das Rampeninkrement
	 * der code war ein macro für die iq-math lib von texas.
	 * dort lässt sich die auflösung der pseudo-floats global einstellen
	 * */
	if (fabs(ramp.Tmp) > ramp.minInc)
		{
		ramp.RampDelayCount++ ;
		if (ramp.RampDelayCount >= ramp.RampDelayMax)
			{
			if(ramp.TargetValue >= ramp.SetpointValue)
				{
				ramp.SetpointValue += ramp.minInc;
				}
			else
				{
				ramp.SetpointValue -= ramp.minInc;
				}
			utils_truncate_number(&ramp.SetpointValue, ramp.RampLowLimit, ramp.RampHighLimit);
			ramp.RampDelayCount = 0;
		}

		}
	/* das nächste inkrement wäre ein überschießen, bzw. rampe fertig*/
	else
	{
		/* Led-Flag setzen */
		//mc_state_fi
	}
}
