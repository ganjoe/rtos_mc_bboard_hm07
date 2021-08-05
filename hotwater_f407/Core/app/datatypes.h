/*
 * datatypes.h
 *
 *  Created on: Jan 2, 2021
 *      Author: danie
 */

#ifndef APP_DATATYPES_H_
#define APP_DATATYPES_H_

/*--stm hal--*/
#include "main.h"
#include "rtc.h"

/*--freertos--*/
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "cmsis_os2.h"
#include "cmsis_os.h"

/*--c library--*/
#include "string.h"
#include "stdio.h"
#include "stdarg.h"
#include "stdlib.h"

#include "utils.h"



/*-------------params----------------*/



/*----------appvars----------------*/

typedef struct
    {
    int divisor;

    uint32_t rampcounter, callcount;
    uint32_t counter, ovf;
    uint32_t oldtick, systick, newtick;
    uint64_t duration, repeat, tickdiff;
    uint32_t timerspeed;	// für timediff berechnung, VOR prescaler
    int flag, flag_delay, flag_reset, init_done;
    float timestep;
    }
    TD_MODFLAG;



/*----------export----------------*/


    void 	 modflag_ovf_callback();

    void 	 mc_tickdiff(TD_MODFLAG *cnt);

    void 	 mc_timediff(TD_MODFLAG *cnt);

extern TD_MODFLAG mf_systick;

#endif /* APP_DATATYPES_H_ */



