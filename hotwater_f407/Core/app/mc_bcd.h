/*
 * mc_bcd.h
 *
 *  Created on: 18.02.2021
 *      Author: pymd02
 *
 *       allgemeines Interface für Motor - Control
 *       Benötigt mc_pwm_if.h
 */

#ifndef INC_MC_BCD_H_
#define INC_MC_BCD_H_

#include "mc_datatypes.h"

typedef struct
{

}TD_MC_BCD6X;

typedef struct
{


}TD_MC_BCD;

/*------------------------------------------------------------------------------
 	DUTY Cycle
------------------------------------------------------------------------------*/
void mc_bcd6x_setduty(float duty, EN_MC_STATES dir, float freq);
void mc_bcd1x_setduty(float duty, EN_MC_STATES dir, float freq);
/*------------------------------------------------------------------------------
 	BEMF Measurement
------------------------------------------------------------------------------*/

#endif /* INC_MC_BCD_H_ */
