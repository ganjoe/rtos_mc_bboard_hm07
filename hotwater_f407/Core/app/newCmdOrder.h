/*
 * newCmdOrder.h
 *
 *  Created on: 07.01.2021
 *      Author: danie
 */

#include "datatypes.h"
#include "mc_datatypes.h"
#include "dbase.h"

#ifndef APP_NEWCMDORDER_H_
#define APP_NEWCMDORDER_H_

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
    TD_TERMINAL_CALLBACKS callbacks[40];
    int callback_write;
    int callback_len;
    int argument_nbr;
}
    TD_CMD;
/*------------api----------------------------
*/
void term_lol_setCallback(
 		TD_CMD*	newcmd,		\
    		const char *command,	\
    		const char *help,	\
    		const char *arg_names,	\
    		void (*cbf)(int argc, const char **argv));

void 	cmd_parse_lobj(TD_CMD* newcmd,TD_LINEOBJ *line);
void	cmd_parse_string(TD_CMD* newcmd,char* string);
void 	cmd_init_callbacks(TD_CMD *newcmd);

/*------------propelli commands--------------*/


void    reset(int argc, const char **argv);
void    settime(int argc, const char **argv);
void    setdate(int argc, const char **argv);
void	setlog(int argc, const char **argv);
void	sdlog(int argc, const char **argv);
void 	help(int argc, const char **argv);
/*------------motor control commands--------------
 * - Contactron-Style Test Interface
 * - Init - Commands

 */
void    init(int argc, const char **argv);
void	duty	(int argc, const char **argv);
void	freq	(int argc, const char **argv);
void	ramp	(int argc, const char **argv);
void	drvgain (int argc, const char **argv);
void	drvrreg (int argc, const char **argv);
void 	drvcal(int argc, const char **argv);

extern TD_CMD newcmd;
#endif /* APP_NEWCMDORDER_H_ */
