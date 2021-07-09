/*
 * mc_dlog.c
 *
 *  Created on: 06.07.2021
 *      Author: pymd02
 */

#ifndef APP_SRC_MC_DLOG_C_
#define APP_SRC_MC_DLOG_C_

#include "../mc_dlog.h"
#include "../mc_datatypes.h"
#include "../terminal.h"

void StartLogUartTask(void *argument)
    {

    for(;;)
	{
	HAL_Delay(termlog.ticks_update_terminal);

	if (termlog.ticks_update_terminal)
	    {
	    term_qPrintf(myTxQueueHandle, "--------\r");
	    dbase_Make(&termlogline, "ADCraw", 0, "Shunt_U", "rise", 0, "%d",mcrt.MotCurrRiseRaw);
	    term_vprintLineObj(myTxQueueHandle, &termlogline);

	    dbase_Make(&termlogline, "ADCraw", 0, "Shunt_U", "fall", 0, "%d",mcrt.MotCurrFallRaw);
	    term_vprintLineObj(myTxQueueHandle, &termlogline);

	    dbase_Make(&termlogline, "ADCSi", 0, "Shunt_U", "rise", 0, "%f",mcrt.MotCurrRiseSi);
	    term_vprintLineObj(myTxQueueHandle, &termlogline);

	    dbase_Make(&termlogline, "ADCSi", 0, "Shunt_U", "fall", 0, "%f",mcrt.MotCurrFallSi);
	    term_vprintLineObj(myTxQueueHandle, &termlogline);
	    }
	else
	    {
	    HAL_Delay(0x1000);
	    }


	}
    }

void dlogSetSelectSingle(TD_DATALOGGA *dlog, EN_LOGITEMS items)
    {
    utils_set_bit_in_Word(&dlog->itemSelectMask, items, 1);
    }
void dlogSetUpdateFreq(TD_DATALOGGA *dlog, float freq)
    {
    if (freq == 0)
	{
	dlog->ticks_update_terminal = 0;
	}
    else
	{
	dlog->ticks_update_terminal = (uint32_t) 1 / freq * 1000;
	}
    }

TD_LINEOBJ termlogline;
TD_DATALOGGA termlog ={.ticks_update_terminal = 1000} ;

#endif /* APP_SRC_MC_DLOG_C_ */
