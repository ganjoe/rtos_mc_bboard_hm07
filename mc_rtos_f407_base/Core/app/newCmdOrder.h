/*
 * newCmdOrder.h
 *
 *  Created on: 07.01.2021
 *      Author: danie
 */

#include "datatypes.h"

#ifndef APP_NEWCMDORDER_H_
#define APP_NEWCMDORDER_H_

void 	cmd_init_callbacks();
void    reset(int argc, const char **argv);
void    settime(int argc, const char **argv);
void    setdate(int argc, const char **argv);
void 	term_lol_parse(TD_LINEOBJ *line);

#endif /* APP_NEWCMDORDER_H_ */
