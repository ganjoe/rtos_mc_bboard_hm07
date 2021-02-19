/*
 * DRV83.h
 *
 *  Created on: Feb 12, 2021
 *      Author: pymd02
 */

#ifndef INC_DRV83_H_
#define INC_DRV83_H_
#include "main.h"

typedef enum
{
	pwm_6x,
	pwm_3x,
	pwm_1x,

}EN_DRV_MODES;

void drv_setPwmMode(EN_DRV_MODES select);


void drv_writeRegister	(uint8_t regNr, uint16_t bitMask);
void drv_readRegister	(uint8_t regNr, uint16_t* bitMask);

#endif /* INC_DRV83_H_ */

