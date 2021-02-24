/*
 * datatypes.h
 *
 *  Created on: Jan 2, 2021
 *      Author: danie
 */

#ifndef APP_DATATYPES_H_
#define APP_DATATYPES_H_


#include "cmsis_os2.h"

/*-------------params----------------*/
#define TD_MODFLAG_TIMERSPEED 80000000	//iterations per second (before prescaler)

#define TD_LINEOBJ_MAX_SSIZE 16
#define TD_LINEOBJ_MAX_HEADERSIZE 8
#define TD_LINEOBJ_MAX_POSTFIXSIZE 5
#define TD_LINEOBJ_MAX_TIMESTRINGSIZE 18
#define TD_LINEOBJ_MAX_FILENAMESIZE 8


/*----------appvars----------------*/

typedef struct
    {
    int divisor;

    uint32_t rampcounter, callcount;
    uint32_t counter, ovf;
    uint64_t oldtick, systick, newtick;
    uint32_t duration, repeat, tickdiff;
    uint32_t timerspeed;	// für timediff berechnung, VOR prescaler
    float freq, duty_sp;

    int flag, flag_delay, flag_reset, init_done;
    }
    TD_MODFLAG;

typedef struct
	{
	const char *command;
	const char *help;
	const char *arg_names;
	void (*cbf)(int argc, const char **argv);
	}
    TD_TERMINAL_CALLBACKS;

typedef struct
    {
    //TODO: replace with freeRTOS datatypes
    char byte_received;
    char* string_rx;
    unsigned int newString;

    /*
    //char* sep;
    //char* eoc;

    //char* string_tx;
    //int uart_buffer_tx_len;
     */

    int flag_newString, flag_newTransmission;
    }
    TD_TERMINAL;

typedef struct
{
    char	timestring[TD_LINEOBJ_MAX_TIMESTRINGSIZE];
    char	filename[TD_LINEOBJ_MAX_FILENAMESIZE];	//rom l999 cmd
    char	string[TD_LINEOBJ_MAX_SSIZE];	//setdate 10 10 10
    char	header[TD_LINEOBJ_MAX_HEADERSIZE];	//tmcu, thot, tcld, fhot, fcld, lvlh, lvlc
    char	postfix[TD_LINEOBJ_MAX_POSTFIXSIZE];	// "%4.3f"
    uint16_t	linenr;
    double 	value;

}
    TD_LINEOBJ;

/*----------export----------------*/

void 	 modflag_ovf_callback();

void 		mc_tickdiff(TD_MODFLAG *cnt);
void 	 mc_timediff(TD_MODFLAG *cnt, float* timesec);

extern TD_MODFLAG mf_systick;

#endif /* APP_DATATYPES_H_ */



