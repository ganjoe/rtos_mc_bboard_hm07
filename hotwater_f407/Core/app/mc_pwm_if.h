/*
 * mc_pwm_if.h
 *
 *  Created on: 18.02.2021
 *      Author: pymd02
 *
 *       allgemeines Low-Level-Interface
 *       mit MCU- und Hardwareabhängiger Implementierung
 */

#ifndef INC_MC_PWM_IF_H_
#define INC_MC_PWM_IF_H_

#include "mc_datatypes.h"


/* Low-Level-Setter für pwm
 * jede funktion hat hartverdrahtete gpio - namen
 * die implementierung braucht dafür eine MCU und CubeMX config.*/

void mc_pwm_bboard_led_1		(uint32_t setpoint);	//blackboard led pwm. zeigt drehrichtung von sim. motor
void mc_pwm_bboard_led_2		(uint32_t setpoint);

void mc_pwm_bcd6x_setduty_u		(uint32_t setpoint);	//Normalbetrieb H-Brücke mit 6-fach pwm
void mc_pwm_bcd6x_setduty_v		(uint32_t setpoint);

void mc_pwm_bcd1x_setduty_u		(uint32_t setpoint);	//Einzelmotor zwischen Vdd und Phase
void mc_pwm_bcd1x_setduty_v		(uint32_t setpoint);

void mc_pwm_bcd3x_setduty_u		(uint32_t setpoint);	//Für Integrierte Halbbrücken-IC mit Enable, bzw. High-Z Signal
void mc_pwm_bcd3x_setduty_v		(uint32_t setpoint);

void mc_pwm_svn3x_setduty_u		(uint32_t setpoint);	//Drehstromlast für 3-fach pwm
void mc_pwm_svn3x_setduty_v		(uint32_t setpoint);
void mc_pwm_svn3x_setduty_w		(uint32_t setpoint);

void mc_pwm_6step_1				(uint32_t setpoint);
void mc_pwm_6step_2				(uint32_t setpoint);
void mc_pwm_6step_3				(uint32_t setpoint);
void mc_pwm_6step_4				(uint32_t setpoint);
void mc_pwm_6step_5				(uint32_t setpoint);
void mc_pwm_6step_6				(uint32_t setpoint);




// setter  für frequenz in hz bzw umrechnung ist dann Architurekturell in mc_datatypes
void mc_set_mcTimerTop(uint32_t period);


#endif /* INC_MC_PWM_IF_H_ */
