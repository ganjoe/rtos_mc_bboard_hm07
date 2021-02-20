/*
 * mc_datatypes.h
 *
 *  Created on: 18.02.2021
 *      Author: pymd02
 *
 *  allgemeine datentypen und funktionen ohne Hardwarebezug.
 *
 */

#ifndef INC_MC_DATATYPES_H_
#define INC_MC_DATATYPES_H_

#include "main.h"
#include "mc_ramp.h"

typedef enum
{
	bb_hm7_blower,
	bb_boardled,

}
	EN_MC_WORKBENCH;

typedef enum
{
	ccw,		//linkslauf
	cw,
	stall,		//leerlauf
	ready,		//aktiv wenn regelschleife (in - out) < minStep
	error,		//wartet auf reset
	standby,	//system energielos
}
	EN_MC_STATES;

typedef enum
{
	bcd6x,		//Normalbetrieb H-Brücke mit 6-fach pwm
	bcd1x,		//Einzelmotor zwischen Vdd und Phase
	bcd3x,		//Für Integrierte Halbbrücken-IC mit Enable, (u.A. IFX, L69xx)
	svn3x,		//Raumzeiger mit übermodulation für 3-fach pwm
}
	EN_MC_MODE;

typedef struct
{
	uint32_t pwm_timer_top;						//Timervalue
	uint16_t pwm_freq_max, pwm_freq_min;		//Hz
	float	pwm_duty_max, pwm_duty_min;			//pu(1)
}
	TD_MC_PWM_PARAMS;

/*-------config und db für workbench----------
 * beschreibt im Sinne eines Savegames das Antriebssystem.
 * Die enthaltenen Typedefs werden nicht woanders erstellt
 * Live-Variablen sind Lokal, oder im TD für Log/Report
 */
typedef struct
{
	TD_MC_PWM_PARAMS pwm;
	EN_MC_STATES	 states;
	EN_MC_MODE		 pwmmode;
	EN_MC_WORKBENCH	 benchsetup;

}TD_MC_PARAMS;



/*-------api für motor workbench----------
 * Allgemeine und MCU sowie Motorunabhängige Funktionen
 */
// init nach reset
void mc_init_default(TD_MC_PARAMS* mcbench);

/* spezielle inits.
TODO::wird durch init-commands speichernd gewählt,
oder neu ausgeführt
ohne auswahl-> terminalmelung
*/

void mc_init_bboard_hm07_boardLedPwm(TD_MC_PARAMS* mcbench);
void mc_init_bboard_hm07_boatblower(TD_MC_PARAMS* mcbench);




/*setzt ein neues top-value für den mc-timer (Hz)
 * Alle pwm-channel sind von einen 16bit-Timer abgeleitet*/
void mc_setfreq(float freq, uint32_t max);

//TODO:: sizeof() nach _init_x starttext
extern TD_MC_PARAMS mcbench;
#endif /* INC_MC_DATATYPES_H_ */