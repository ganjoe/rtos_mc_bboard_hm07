/*
 * newCmdOrder.h
 *
 *  Created on: 07.01.2021
 *      Author: danie
 */

#include "datatypes.h"
#include "mc_datatypes.h"

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
/*------------api----------------------------
*/

void 	cmd_parse_lobj(TD_LINEOBJ *line);
void 	cmd_parse(char* string);

/*------------propelli commands--------------*/

void 	cmd_init_callbacks();
void    reset(int argc, const char **argv);
void    settime(int argc, const char **argv);
void    setdate(int argc, const char **argv);


/*------------motor control commands--------------
 * - Contactron-Style Test Interface
 * - Init - Commands
 *
 *
 */
void	init	(int argc, const char **argv);

void	duty	(int argc, const char **argv);
void	freq	(int argc, const char **argv);
void	ramp	(int argc, const char **argv);
void	speed	(int argc, const char **argv);
void	mspd	(int argc, const char **argv);
void	dir		(int argc, const char **argv);
void	init	(int argc, const char **argv);



#endif /* APP_NEWCMDORDER_H_ */
