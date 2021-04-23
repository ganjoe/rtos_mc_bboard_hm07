/*
 * dbase.h
 *
 *  Created on: 22.04.2021
 *      Author: danie
 *
 *	22.4.21 - eigene .c/.h
 */

#include "main.h"
#include "datatypes.h"


#ifndef APP_DBASE_H_
#define APP_DBASE_H_

#define TD_LINEOBJ_MAX_SSIZE 16
#define TD_LINEOBJ_MAX_HEADERSIZE 8
#define TD_LINEOBJ_MAX_POSTFIXSIZE 5
#define TD_LINEOBJ_MAX_TIMESTRINGSIZE 18
#define TD_LINEOBJ_MAX_FILENAMESIZE 8
//array size for snprintf
#define UART_PRINTBUFFER 	64

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

void dbase_parse_cmd(TD_LINEOBJ *line);

void dbase_StoreSD(TD_LINEOBJ *line);

void dbase_LoadQueue ( osMessageQueueId_t QueueHandle, TD_LINEOBJ *line);

BaseType_t dBase_StoreQueue(osMessageQueueId_t QueueHandle, TD_LINEOBJ *line);

void dbase_Make(TD_LINEOBJ *line,
			const char* filename,
			const char* string,
			const char* header,
			const char* postfix,
			uint16_t* linenr,
			const char 	*fmt,
			... );


#endif /* APP_DBASE_H_ */
