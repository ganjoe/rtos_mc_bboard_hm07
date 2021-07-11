/*
 * newCmdOrder.c
 *
 *  Created on: 07.01.2021
 *      Author: ganjoe
 *
 *      rewrite aus vredder bldc firmware
 *
 *
 *      23.4.21 - eigener namesraum für datentypen
 *
 *
 */

#include "main.h"
#include "rtc.h"

#include "../newCmdOrder.h"
#include "../datatypes.h"
#include "../terminal.h"
#include "../mc_ramp.h"
#include "../mc_dlog.h"


/* muss in diesen scope stehen */
RTC_TimeTypeDef time;
RTC_DateTypeDef date;

extern osMessageQueueId_t myTxQueueHandle;
extern osSemaphoreId_t myCountNewCmdHandle;
extern osSemaphoreId_t myCmdLineObjQueueHandle;

/*------------propelli commands-------------------*/

void reset(int argc, const char **argv)
    {
    float f = -1;

    if (argc == 2)
	{
	sscanf(argv[1], "%f", &f);
	term_qPrintf(myTxQueueHandle, "\r[parseCmd] reset ok:%5fs ok\r", f);
	HAL_NVIC_SystemReset();
	}
    }

void settime(int argc, const char **argv)
    {

    int h = -1;	//
    int m = -1;	//
    int s = -1;	//
    if (argc == 4)
	{
	sscanf(argv[1], "%d", &h);
	sscanf(argv[2], "%d", &m);
	sscanf(argv[3], "%d", &s);
	int wparam = 0;
	wparam = utils_truncate_number_int(&h, 0, 23);
	wparam = utils_truncate_number_int(&m, 0, 59);
	wparam = utils_truncate_number_int(&s, 0, 59);
	if (wparam)
	    {
	    //term_qPrintf(&myTxQueueHandle, "[cmd settime][Fehler] Range ist 23 59 59\r");
	    }
	else
	    {
	    term_qPrintf(myTxQueueHandle, "\[parse ok]\r");
	    time.Hours = h;
	    time.Minutes = m;
	    time.Seconds = s;
	    time.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	    time.StoreOperation = RTC_STOREOPERATION_RESET;

	    HAL_RTC_SetTime(&hrtc, &time, RTC_FORMAT_BIN);
	    HAL_RTC_SetDate(&hrtc, &date, RTC_FORMAT_BIN);
	    }
	}
    else
	{
	term_qPrintf(myTxQueueHandle,
		"\rwrong! arguments are DD MM YY. YY>=20\r");
	}

    }

void setdate(int argc, const char **argv)
    {
    int d = -1;	//
    int m = -1;	//
    int y = -1;	//
    if (argc == 4)
	{
	int wparam;
	sscanf(argv[1], "%d", &d);
	sscanf(argv[2], "%d", &m);
	sscanf(argv[3], "%d", &y);
	wparam = utils_truncate_number_int(&d, 0, 32);
	wparam = utils_truncate_number_int(&m, 0, 12);
	wparam = utils_truncate_number_int(&y, 20, 65);
	if (wparam)
	    {
	    term_qPrintf(&myTxQueueHandle, "\rrange ist 32 12 65\r");
	    }
	else
	    {
	    date.Month = m;
	    date.Date = d;
	    date.Year = y;
	    date.WeekDay = RTC_WEEKDAY_SUNDAY;
	    HAL_RTC_SetTime(&hrtc, &time, RTC_FORMAT_BIN);
	    HAL_RTC_SetDate(&hrtc, &date, RTC_FORMAT_BIN);
	    term_qPrintf(&myTxQueueHandle, "[cmd setdate] ok\r");
	    }
	}
    else
	{
	term_qPrintf(&myTxQueueHandle, "\r3 argumente DD MM YY\r");
	}
    }

void setlog(int argc, const char **argv)
    {
    float f = -1;
    if (argc == 2)
	{
	sscanf(argv[1], "%f", &f);
	term_qPrintf(myTxQueueHandle, "\r[parseCmd] termlog: ok", f);
	dlogSetUpdateFreq(&termlog, f);

	}
    }

void sdlog(int argc, const char **argv)
    {

    }

void help(int argc, const char **argv)
    {
    for (int var = 0; var < newcmd.callback_write; ++var)
	{
	term_qPrintf(myTxQueueHandle, "\rCMD %s ARG %s DESC %s \r",
			    newcmd.callbacks[var].command,
			    newcmd.callbacks[var].arg_names,
			    newcmd.callbacks[var].help);
	HAL_Delay(10);

	}
    }


/*------------motor control commands--------------*/

void duty(int argc, const char **argv)
    {
    float f = -1;
    if (argc == 2)
	{
	sscanf(argv[1], "%f", &f);
	term_qPrintf(myTxQueueHandle, "\r[parseCmd] duty: ok", f);
	mcbench.ramp->RampStepLimit = 1;
	mcbench.ramp->Target = (f);

	}
    }


void freq(int argc, const char **argv)
    {
    float d = -1;
    if (argc == 2)
	{
	sscanf(argv[1], "%f", &d);
	term_qPrintf(myTxQueueHandle, "\r[parseCmd] freq: ok");
	mcbench.pwm->freq = (d);
	}
    }

void ramp(int argc, const char **argv)
    {
    float f = -1;
    float g = -1;
    if (argc == 3)
	{
	sscanf(argv[1], "%f", &f);
	sscanf(argv[2], "%f", &g);
	term_qPrintf(myTxQueueHandle, "\r[parseCmd] ramp: ok");
	mcbench.ramp->Target = (f);
	mcbench.ramp->gain = g;
	}
    else
	{
	term_qPrintf(myTxQueueHandle, "\r[parseCmd] Fehler Anzahl Argumente");
	}

    }

void init(int argc, const char **argv)
{
    int d = -1;
    if (argc == 2)
	{
	sscanf(argv[1], "%d", &d);
	term_qPrintf(myTxQueueHandle, "\r[parseCmd] mcbench: ok");
	mcbench.benchsetup = (d);
	}
}

void drvrreg(int argc, const char **argv)
    {
    int d = -1;
    uint16_t reg = 0;
    if (argc == 2)
	{
	sscanf(argv[1], "%d", &d);
	if(!utils_truncate_number_int(&d, 0, 7))
	    {
	    term_qPrintf(myTxQueueHandle, "\r[parseCmd] drvrreg: ok");
	    drv_readRegister(d, &reg);
	    term_qPrintf(myTxQueueHandle, "\r%d",reg);
	    }
	else
	    {
	    term_qPrintf(myTxQueueHandle, "\r[parseCmd] drvrreg: arg(0:15)");
	    }
	}
    else
	{
	term_qPrintf(myTxQueueHandle, "\r[parseCmd] drvrreg: arg(0:15)");
	}
    }

void drvgain(int argc, const char **argv)
    {
    int d = -1;
    if (argc == 2)
	{
	sscanf(argv[1], "%d", &d);
	switch (d)
	    {
	    case drv_sgain_5: drv.csa_shunt.csa_gain = drv_sgain_5;
		term_qPrintf(myTxQueueHandle, "\r[parseCmd] drvgain: drv_sgain_5 ok");break;
	    case drv_sgain_10: drv.csa_shunt.csa_gain = drv_sgain_10;
		term_qPrintf(myTxQueueHandle, "\r[parseCmd] drvgain: drv_sgain_10 ok");break;
	    case drv_sgain_20: drv.csa_shunt.csa_gain = drv_sgain_20;
		term_qPrintf(myTxQueueHandle, "\r[parseCmd] drvgain: drv_sgain_20 ok");break;
	    case drv_sgain_40: drv.csa_shunt.csa_gain = drv_sgain_40;
		term_qPrintf(myTxQueueHandle, "\r[parseCmd] drvgain: drv_sgain_40 ok");break;
	    default:
		term_qPrintf(myTxQueueHandle, "\r[parseCmd] drvgain: gain(0:4)");
		break;
	    }
	drv_setShuntGain(&drv);
	}
    }

void drvcal(int argc, const char **argv)
    {
    float cal = -1;
    if (argc == 2)
	{
	sscanf(argv[1], "%f", &cal);
	term_qPrintf(myTxQueueHandle, "\r[parseCmd] drvcal: ok");
	drv_calib_csa(&drv, cal, mcrt.MotCurrRiseRaw);
	term_qPrintf(myTxQueueHandle, "\r[parseCmd] drvcal: new lsb set");

	}
    }


/*-----------------------------------------------*/

void StartCmdTask(void *argument)
{
    for (;;)
	{
	if( xSemaphoreTake( myCountNewCmdHandle, osWaitForever)==pdPASS)
	    {
	    TD_LINEOBJ line;

	    dbase_LoadQueue(myCmdLineObjQueueHandle, &line);

	    cmd_parse_lobj(&newcmd, &line);
	    }
	}
  /* USER CODE END StartCmdTask */
}

/*-----------------------------------------------*/
void cmd_init_callbacks(TD_CMD *asdf)
    {
    asdf->callback_len = 40;
    asdf->argument_nbr = 4;

    term_lol_setCallback(asdf, "reset", "mcu reset", "1,0 uint", reset);
    term_lol_setCallback(asdf, "settime", "mcu reset", "3 uint", settime);
    term_lol_setCallback(asdf, "setdate", "mcu reset", "3 uint", setdate);
    term_lol_setCallback(asdf, "setlog", "livedaten terminal", "update freq", setlog);
    term_lol_setCallback(asdf, "sdlog", "livedaten flash", "updatefreq", sdlog);
    term_lol_setCallback(asdf, "help", "registrierte befehle", "kein", help);

    term_lol_setCallback(asdf, "duty", "pwm duty-cycle (-)ccw", "float",  duty);
    term_lol_setCallback(asdf, "freq", "pwm freq hz", "uint16", freq);
    term_lol_setCallback(asdf, "ramp", "zielwert 0..1, bezug(s)", "2 floats",  ramp);
    term_lol_setCallback(asdf, "init", "setup 0,1,..", "1 int",  init);
    term_lol_setCallback(asdf, "drvgain", "shuntgain vom drv83", "0:5",  drvgain);
    term_lol_setCallback(asdf, "drvrreg", "drv83 register read", "0:15",  drvrreg);
    term_lol_setCallback(asdf, "drvcal", "drv83 csa calibration", "cal current(float)",  drvcal);

    }
void cmd_parse_lobj(TD_CMD *newcmd, TD_LINEOBJ *line)
    {
    cmd_parse_string(newcmd, line->string);
    }
void cmd_parse_string(TD_CMD *newcmd, char *string)
    {
    char strbuffer[newcmd->callback_len];

    char* strbufferptr = &strbuffer;

    int ArgCount = 0;

    char *ptrArgBuffer[newcmd->argument_nbr]; //max arguemtns

    //cmd ist der erste stringabschnitt von links
    strbufferptr = strtok(string, strdup(" "));

    //argumente separieren, und in ptr-array speichern
    while (strbufferptr && ArgCount < 4)
	{
	ptrArgBuffer[ArgCount++] = strbufferptr;

	strbufferptr = strtok(0, strdup(" "));
	}

    /*
     if (argc == 0)
     {
     term_qPrintf(myTxQueueHandle, "No command received\n");
     return;
     }
     if (strcmp(argv[0], "help") == 0)
     {
     term_qPrintf(myTxQueueHandle, "registered commands:\n");

     for (int i = 0; i < callback_write; i++)
     {
     term_qPrintf(myTxQueueHandle, callbacks[i].command);
     term_qPrintf(myTxQueueHandle, "\rhelp: ");
     term_qPrintf(myTxQueueHandle, callbacks[i].help);
     term_qPrintf(myTxQueueHandle, "\r");
     }
     }
     */
    for (int i = 0; i < newcmd->callback_write; i++)
	{
	if (newcmd->callbacks[i].cbf != 0
		&& strcmp(ptrArgBuffer[0], newcmd->callbacks[i].command) == 0)
	    {
	    newcmd->callbacks[i].cbf(ArgCount, (const char**) ptrArgBuffer);
	    return;
	    }
	}    //
    }
void term_lol_setCallback(TD_CMD *newcmd, const char *command, const char *help,const char *arg_names, void (*cbf)(int argc, const char **argv))
    {
    int callback_num = newcmd->callback_write;
    for (int i = 0; i < newcmd->callback_write; i++)
	{
	// First check the address in case the same callback is registered more than once.
	if (newcmd->callbacks[i].command == command)
	    {
	    callback_num = i;
	    break;
	    }
	// Check by string comparison.
	if (strcmp(newcmd->callbacks[i].command, command) == 0)
	    {
	    callback_num = i;
	    break;
	    }
	// Check if the callback is empty (unregistered)
	if (newcmd->callbacks[i].cbf == 0)
	    {
	    callback_num = i;
	    break;
	    }
	}
    newcmd->callbacks[callback_num].command = command;
    newcmd->callbacks[callback_num].help = help;
    newcmd->callbacks[callback_num].arg_names = arg_names;
    newcmd->callbacks[callback_num].cbf = cbf;

    if (callback_num == newcmd->callback_write)
	{
	newcmd->callback_write++;
	if (newcmd->callback_write >= newcmd->callback_len)
	    {
	    newcmd->callback_write = 0;
	    }
	}
    }

TD_CMD newcmd;
