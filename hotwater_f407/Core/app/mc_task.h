/*
 * mc_task.h
 *
 *  Created on: 18.02.2021
 *      Author: pymd02
 *
 *      Motor - Control - App
 *      systick wird über CubeMx - FreeRtos festgelegt
 *      abhängig von den headern für jeden Control - Block
 */

/*
 * BCD mit Ankerstrom und Bemf-Regelung
 *
 * Eingangsgrößen TD_MC_IF:
 * poti0	pwm-freq
 * poti1	rampe-rauf
 * poti2	rampe-runter
 * poti3	speed
 * poti4	max speed
 * switch0	cr
 * switch1	ccr
 * switch2	reset/init/detect
 * led0		status
 * led1		ccw
 * led2		cw
 * led2		fehler
 *
 */

#ifndef INC_MC_TASK_H_
#define INC_MC_TASK_H_

#include "mc_ramp.h"

void mc_setup();

void mc_loop();


void StartDefaultTask(void *argument);

extern RMPCNTL potiramp;
#endif /* INC_MC_TASK_H_ */
