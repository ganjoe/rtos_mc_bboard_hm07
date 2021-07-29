/*
 * mc_config.h
 *
 *  Created on: Jul 27, 2021
 *      Author: pymd02
 */

#ifndef APP_MC_CONFIG_H_
#define APP_MC_CONFIG_H_

#include "mc_datatypes.h"

//schreibt standardwerte in die mc-config
int confgen_setdefaults(TD_MC_PARAMS *mc_params);

//mc-config wird mit werten aus buffer überschrieben
int confgen_demultiplex_config(TD_MC_PARAMS *mc_params, uint8_t* buffer);

//mc- config wird in buffer geschrieben
int confgen_multiplex_config(uint8_t* buffer, TD_MC_PARAMS *data);

//mc - laufzeitvariablen werden in buffer geschrieben
int confgen_multiplex_livedata(uint8_t* buffer, TD_MC_LIVE *data);

#endif /* APP_MC_CONFIG_H_ */
