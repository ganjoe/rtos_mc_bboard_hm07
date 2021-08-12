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
	    dbase_Make(&termlogline, "ADCraw", 0, "Shunt_U", "rise", 0, "%d",mcrt.adc_shunt_u_rise);
	    term_vprintLineObj(myTxQueueHandle, &termlogline);

	    dbase_Make(&termlogline, "ADCraw", 0, "Shunt_V", "rise", 0, "%d",mcrt.adc_shunt_v_rise);
	    term_vprintLineObj(myTxQueueHandle, &termlogline);

	    dbase_Make(&termlogline, "ADCraw", 0, "Bus_U", "rise", 0, "%d",mcrt.adc_phase_u_bus);
	    term_vprintLineObj(myTxQueueHandle, &termlogline);

	    dbase_Make(&termlogline, "ADCraw", 0, "Bus_V", "rise", 0, "%d",mcrt.adc_phase_v_bus);
	    term_vprintLineObj(myTxQueueHandle, &termlogline);

	    dbase_Make(&termlogline, "ADCraw", 0, "Shunt_U", "fall", 0, "%d",mcrt.adc_shunt_u_fall);
	    term_vprintLineObj(myTxQueueHandle, &termlogline);

	    dbase_Make(&termlogline, "ADCraw", 0, "Shunt_V", "fall", 0, "%d",mcrt.adc_shunt_v_fall);
	    term_vprintLineObj(myTxQueueHandle, &termlogline);

	    dbase_Make(&termlogline, "ADCraw", 0, "EMK_U", "fall", 0, "%d",mcrt.adc_phase_u_emk);
	    term_vprintLineObj(myTxQueueHandle, &termlogline);

	    dbase_Make(&termlogline, "ADCraw", 0, "EMK_V", "fall", 0, "%d",mcrt.adc_phase_v_emk);
	    term_vprintLineObj(myTxQueueHandle, &termlogline);

	    dbase_Make(&termlogline, "SI", 0, "Busvolt", "pwm", 0, "%f",mcrt.phase_rise_si);
	    term_vprintLineObj(myTxQueueHandle, &termlogline);

	    dbase_Make(&termlogline, "SI", 0, "Strom", "cw/ccw", 0, "%f",mcrt.curr_rise_si);
	    term_vprintLineObj(myTxQueueHandle, &termlogline);

	    }
	else
	    {
	    HAL_Delay(0x1000);
	    }


	}
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

void dlogPause(TD_DATALOGGA *dlog)
    {
    dlog->ticks_update_terminal_backup = dlog->ticks_update_terminal;
    dlog->ticks_update_terminal = 0;
    term_qPrintf(myTxQueueHandle, "\rdlog pause");
    }
void dlogResume(TD_DATALOGGA *dlog)
    {
    term_qPrintf(myTxQueueHandle, "dlog resume\r");
    dlog->ticks_update_terminal = dlog->ticks_update_terminal_backup;

    }

TD_LINEOBJ termlogline;
TD_DATALOGGA termlog;

#endif /* APP_SRC_MC_DLOG_C_ */
