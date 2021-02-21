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

/* Includes für Propelli	 */
/* Includes für Motorcontrol */

/* -----Motor Control Loop---------
 muss beider codegenerierung als "weak" angegeben werden, ansonsten doppelt definiert

Die Loop ist nicht pwm-synchron, aber die ADC-Messung.
Während jedes Durchlaufes werden die ADC-Daten durch
ein ISR-Callback gebuffert und bei Beginn gemittelt.

abfrage von TD_MC_IF Variablen (durch einen anderen Task geupdated)
ADC-FILTER - update von den adc-queues
USER-RAMP - "Poti-Rampe", ggf. der äußere Regle
FAST-RAMP - "Safty-Rampe", ggf. der innere Regler
 PWM & FREQ - für PWM Timer setzen */

void StartMcTask(void *argument)
{
	mc_init_default(&mcbench);
	mc_init_bboard_hm07_boardLedPwm(&mcbench);

    while (1)
	{

	}
}

