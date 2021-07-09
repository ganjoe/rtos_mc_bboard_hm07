/*
 * mc_dlog.h
 *
 *  Created on: 06.07.2021
 *      Author: pymd02
 */

#ifndef APP_MC_DLOG_H_
#define APP_MC_DLOG_H_

#include "datatypes.h"
#include "dbase.h"

typedef enum
{
    RISECURRENT,
    FALLCURRENT,
    BUSVOLT,
}
    EN_LOGITEMS;

typedef struct
{
    uint32_t ticks_update_terminal;
    uint16_t itemSelectMask;
}
    TD_DATALOGGA;

void dlogSetSelectSingle(TD_DATALOGGA *dlog, EN_LOGITEMS items);
void dlogSetSelectMulti(TD_DATALOGGA *dlog);
void dlogSetUpdateFreq(TD_DATALOGGA *dlog, float freq);

extern TD_DATALOGGA termlog;
extern TD_LINEOBJ termlogline;

#endif /* APP_MC_DLOG_H_ */
