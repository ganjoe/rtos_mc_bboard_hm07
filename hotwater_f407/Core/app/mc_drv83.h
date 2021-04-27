/*
 * DRV83.h
 *
 *  Created on: Feb 12, 2021
 *      Author: pymd02
 */

#ifndef INC_DRV83_H_
#define INC_DRV83_H_

#include "main.h"
// wenn du es gefunden hast, schreibs gleich woanders hin !
#define DRV_WRITE_FAIL_COUNT 3

typedef enum
{
	drv_pwm_6x,
	drv_pwm_3x,
	drv_pwm_1x,
}
	EN_DRV_MODE_PWM;

typedef enum
{
	/* Shunt amplifier gain */
	drv_sgain_5,
	drv_sgain_10,
	drv_sgain_20,
	drv_sgain_40,

}
	EN_DRV_MODE_SHNT;

typedef enum
{
	/* sense overcurrent protection */
	drv_ocp_250mV,
	drv_ocp_500mV,
	drv_ocp_750mV,
	drv_ocp_1000mV,
}
    EN_DRV_MODE_OLP;

typedef enum
{
    drv_shunt_unidirectional,
    drv_shunt_bidirectinal

}
    EN_DRV_MODE_OFFSET;

    /* Registeradressen sind durchnummeriert */
typedef enum
    {
    	faultstatus = 0,
    	vgsStatus,
    	driverControl,
    	gateDriveHS,
    	gateDriveLS,
    	OCPcontrol,
    	CSAcontrol,
    }
    EN_DRV_ADDR;

typedef struct
    	{
    	uint16_t shadowRegister[8];
    	EN_DRV_ADDR regAdress;
    	EN_DRV_MODE_PWM modeSelect;
    	EN_DRV_MODE_OFFSET opref;
    	EN_DRV_MODE_OLP OLshuntvolts;
    	EN_DRV_MODE_SHNT csa_gain;
    	}
    TD_DRV83;

void drv_setPwmMode	(TD_DRV83 *select);
void drv_setShuntGain	(TD_DRV83 *select);
void drv_setShuntSign	(TD_DRV83 *select);
void drv_setOvrLoadProt	(TD_DRV83 *select);
void drv_en_drv		(TD_DRV83 *select, int enable);
int drv_check_fault	(TD_DRV83 *select);
void drv_readRegister(uint16_t regNr, uint16_t *data);
void drv_writeCompareReg(uint8_t regNr, uint16_t reg);
void drv_csPulse();
/* hal init */
void drv_init_spi();

extern TD_DRV83 drv;

#endif /* INC_DRV83_H_ */

