/*
 * mc_datatypes.h
 *
 *  Created on: 18.02.2021
 *      Author: pymd02
 *
 *  allgemeine, globale, datentypen und funktionen ohne Hardwarebezug.
 *
 */

#ifndef INC_MC_DATATYPES_H_
#define INC_MC_DATATYPES_H_

typedef enum
{
	ccw,
	cw,
	stall,
	error,
	standby
}EN_MC_STATES;

/* TODO: als macro schreiben */

/*setzt ein neues top-value für den mc-timer (Hz)*/
void mc_setfreq(float freq, uint32_t max);


#endif /* INC_MC_DATATYPES_H_ */
