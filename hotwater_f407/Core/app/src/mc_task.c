/*
 * mc_task.c
 *
 *  Created on: Feb 19, 2021
 *      Author: danie
 */

/* Includes für FreeRtos - Tasks und so */

#include "../mc_task.h"
#include "../mc_datatypes.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* 	Includes für Propelli	 */
/* Includes für Motorcontrol */

#include "../mc_ramp.h"


/* -----Motor Control Loop---------
 muss beider codegenerierung als "weak" angegeben werden, ansonsten doppelt definiert

 mc_timediff :
 dynamische schrittweite als basis, dh. jewails die Zeit seit letzten aufruf der mc - loop
 es wird ein unendlich - zähler auf hardwarebasis verwendet
 (overflow - zähler in der isr) um eine zeitmessung zu realisieren.
 Einheit sind timer - ticks, die ggf. in zeit umgerechnet werden

Die Loop ist nicht pwm-synchron, aber die ADC-Messung.
Bei jeden mc - loop aufruf steht ein buffer mit unverarbeiteten adc-werten bereit
*/

void StartMcTask(void *argument)
{
	mc_init_default(&mcbench);
	mc_init_bboard_hm07_boardLedPwm(&mcbench);
	mc_ramp_init(&mcbench.potiramp);
	mf_systick.timerspeed = mcbench.pwm.pwm_timer_speed;

	float timestep;

    while (1)
    	{
    	mc_timediff(&mf_systick, &timestep);

    	mcbench.potiramp.RampTimestep = timestep;

    	mc_ramp		(&mcbench.potiramp);

    	mcbench.pwm.pwm_duty = mcbench.potiramp.SetpointValue;

		mc_setduty	(&mcbench);
    	}

}

