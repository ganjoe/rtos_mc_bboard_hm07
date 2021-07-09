/*
 * DRV83.c
 *
 *  Created on: Feb 12, 2021
 *      Author: pymd02
 *
 *      Treiber für CubeMX - Hal konfigurierte SPI hardware
 *      standard - konfig ändern:
 *      - 16bit Datenbreite
 *      - 2 Edges
 *      DRV Enable muss high sein
 *      zwischen Read und Write muss CS auf high pulsen (1us reicht)
 *      der MISO - pullup kann niederohmig sein (1k) (muss, bei höhehren baud)
 *      Datenrate kann auch superlangsam sein (1hz)
 */

#include "spi.h"

#include "../mc_drv83.h"
#include "../utils.h"
#include "../terminal.h"
#include "adc.h"




void drv_setPwmMode(TD_DRV83 *select)
    {
    uint16_t regbuffer = 0;

    drv_readRegister(driverControl, &regbuffer);
    //term_qPrintf(&myTxQueueHandle, "\r[drv_setPwmMode]");

    switch (select->modeSelect)
	{
    case drv_pwm_1x:
	utils_set_bit_in_Word(&regbuffer, 5, 0);
	utils_set_bit_in_Word(&regbuffer, 6, 1);
	//term_qPrintf(&myTxQueueHandle, "\rdrv_pwm_1xr");
	break;
    case drv_pwm_3x:
	utils_set_bit_in_Word(&regbuffer, 5, 1);
	utils_set_bit_in_Word(&regbuffer, 6, 0);
	//term_qPrintf(&myTxQueueHandle, "\rdrv_pwm_3x");
	break;
    case drv_pwm_6x:
	utils_set_bit_in_Word(&regbuffer, 5, 0);
	utils_set_bit_in_Word(&regbuffer, 6, 0);
	//term_qPrintf(&myTxQueueHandle, "\rdrv_pwm_6x");
	break;

    default:
	//term_qPrintf(&myTxQueueHandle, "[drv_setPwmMode] default:drv_pwm_6x");
	break;
	}
    drv_writeCompareReg(CSAcontrol, regbuffer);
    }

void drv_setShuntSign(TD_DRV83 *select)
    {
   // uint16_t bitMask = 0;
    uint16_t regbuffer = 0;

    drv_readRegister(CSAcontrol, &regbuffer);

    switch (select->opref)
	{
    case drv_shunt_unidirectional:
	utils_set_bit_in_Word(&regbuffer, 9, 0);
	//term_qPrintf(&myTxQueueHandle,
		//	"[drv_setShuntSign] drv_shunt_unidirectional\r");
	break;
    case drv_shunt_bidirectinal:
	utils_set_bit_in_Word(&regbuffer, 9, 1);
	//term_qPrintf(&myTxQueueHandle,
	//	"[drv_setShuntSign]drv_shunt_bidirectinal ok\r");
	break;
	}

    //regbuffer |= bitMask;
    drv_writeCompareReg(CSAcontrol, regbuffer);

    }

void drv_setShuntGain(TD_DRV83 *select)
    {

    uint16_t regbuffer = 0;

    drv_readRegister(CSAcontrol, &regbuffer);

    switch (select->csa_shunt.csa_gain)
	{
    case drv_sgain_5:
	utils_set_bit_in_Word(&regbuffer, 6, 0);
	utils_set_bit_in_Word(&regbuffer, 7, 0);
	select->csa_shunt.lsb = select->csa_shunt.Ilsb[drv_sgain_5];
	//term_qPrintf(&myTxQueueHandle, "\r[drv_setShuntGain] x5 | ILSB: %f",select->csa_shunt.Ilsb[drv_sgain_5]);
	break;
    case drv_sgain_10:
	utils_set_bit_in_Word(&regbuffer, 6, 1);
	utils_set_bit_in_Word(&regbuffer, 7, 0);
	select->csa_shunt.lsb = select->csa_shunt.Ilsb[drv_sgain_10];
	//term_qPrintf(&myTxQueueHandle, "\r[drv_setShuntGain] x10");
	break;
    case drv_sgain_20:
	utils_set_bit_in_Word(&regbuffer, 6, 0);
	utils_set_bit_in_Word(&regbuffer, 7, 1);
	select->csa_shunt.lsb = select->csa_shunt.Ilsb[drv_sgain_20];
	//term_qPrintf(&myTxQueueHandle, "\r[drv_setShuntGain] x20");
	break;
    case drv_sgain_40:
	utils_set_bit_in_Word(&regbuffer, 6, 1);
	utils_set_bit_in_Word(&regbuffer, 7, 1);
	select->csa_shunt.lsb = select->csa_shunt.Ilsb[drv_sgain_40];
	//term_qPrintf(&myTxQueueHandle, "\r[drv_setShuntGain] x40");
	break;
	}

    drv_writeCompareReg(CSAcontrol, regbuffer);
    }

void drv_setOvrLoadProt(TD_DRV83 *select)
    {
    uint16_t regbuffer = 0;

    drv_readRegister(CSAcontrol, &regbuffer);

    //term_qPrintf(&myTxQueueHandle, "\r[drv_setOvrCurrProt] ");

    switch (select->OLshuntvolts)
	{
    case drv_ocp_250mV:
	utils_set_bit_in_Word(&regbuffer, 0, 0);
	utils_set_bit_in_Word(&regbuffer, 1, 0);
	//term_qPrintf(&myTxQueueHandle, "drv_ocp_250mV");
	break;
    case drv_ocp_500mV:
	utils_set_bit_in_Word(&regbuffer, 0, 1);
	utils_set_bit_in_Word(&regbuffer, 1, 0);
	//term_qPrintf(&myTxQueueHandle, "drv_ocp_500mV");
    case drv_ocp_750mV:
	utils_set_bit_in_Word(&regbuffer, 0, 0);
	utils_set_bit_in_Word(&regbuffer, 1, 1);
	//term_qPrintf(&myTxQueueHandle, "drv_ocp_750mV");
    case drv_ocp_1000mV:
	utils_set_bit_in_Word(&regbuffer, 0, 1);
	utils_set_bit_in_Word(&regbuffer, 1, 1);
	//term_qPrintf(&myTxQueueHandle, "drv_ocp_1000mV");
	break;
	}
    utils_set_bit_in_Word(&regbuffer, 5, 1);
   // term_qPrintf(&myTxQueueHandle,
//	    "\r[drv_setOvrCurrProt] latched nFault enabled ");

    drv_writeCompareReg(CSAcontrol, regbuffer);
    }

uint32_t drv_adc_ref()
    {
    uint32_t sum = 0;
    uint32_t samples = 1024;
    uint32_t ovrsample = 256;
    uint32_t adcresolution = 0xfff;
    uint32_t maxsamples = 0xffffffff / (ovrsample *adcresolution);
    int did_trunc;
    did_trunc = utils_truncate_number_int32(samples, 1, maxsamples);

     for (int var = 0; var < samples; ++var)
 	{
 	HAL_ADC_Start(&hadc1);
 	if (HAL_ADC_PollForConversion(&hadc1, HAL_TIMEOUT) == HAL_OK)
 	    {
 	   sum += HAL_ADC_GetValue(&hadc1)*ovrsample;

 	    }
 	}
     sum/=samples;

     return  sum /ovrsample;
    }



void drv_en_drv(int enable)
    {
    term_qPrintf(&myTxQueueHandle, "\r[drv_en_drv]");
    if (enable)
	{
	HAL_GPIO_WritePin(drv_en_GPIO_Port, drv_en_Pin, 1);
	HAL_Delay(1);
	HAL_GPIO_WritePin(drv_en_GPIO_Port, drv_en_Pin, 0);
	HAL_Delay(1);
	HAL_GPIO_WritePin(drv_en_GPIO_Port, drv_en_Pin, 1);
	term_qPrintf(&myTxQueueHandle, "\rreset & enable");
	}
    else
	{
	HAL_GPIO_WritePin(drv_en_GPIO_Port, drv_en_Pin, 0);
	term_qPrintf(&myTxQueueHandle, "\rdisabled");
	}

    }
int drv_check_fault()
    {
    return !HAL_GPIO_ReadPin(drv_fault_GPIO_Port, drv_fault_Pin);
    }

/*-------------low level--------------------*/

void drv_writeRegister(uint8_t regNr, uint16_t bitMask)
    {

    uint16_t tword;

    //adressnummer setzen (4 bit) und an position vor das rw-bit schieben
    tword = regNr;
    tword <<= 11;
    // rw -bit setzen(15)
    utils_set_bit_in_Word(&tword, 15, 0);
    utils_set_bits_in_Word(&tword, bitMask, 1);
    drv_csPulse();
    HAL_SPI_Transmit(&hspi1, (uint8_t*) &tword, 1, HAL_TIMEOUT);
    }
void drv_writeCompareReg(uint8_t regNr, uint16_t reg)
    {
    uint16_t regbuffer = 0;
    uint8_t errcounter = 0;

    if (regbuffer != reg)
	{
	drv_writeRegister(regNr, reg);
	drv_csPulse();
	drv_readRegister(regNr, &regbuffer);

	errcounter++;
	if (errcounter == DRV_WRITE_FAIL_COUNT)
	    {
	    term_qPrintf(myTxQueueHandle,
		    "\r[drv_writeCompareReg] DRV_WRITE_FAIL_COUNT %d",
		    DRV_WRITE_FAIL_COUNT);
	    HAL_Delay(10);
	    Error_Handler();
	    }
	}

    }
void drv_csPulse()
    {
    //TODO: delay.c aus propelli-head mergen und mit wait_isr ergänzen
    HAL_GPIO_WritePin(drv_cs_GPIO_Port, drv_cs_Pin, 0);
    HAL_GPIO_WritePin(drv_cs_GPIO_Port, drv_cs_Pin, 1);
    int var = 0;
    for (; var < 0xF; ++var)
	{
	var += 1;
	}
    HAL_GPIO_WritePin(drv_cs_GPIO_Port, drv_cs_Pin, 0);

    for (; var < 0xF; ++var)
	{
	var += 1;
	}
    }
void drv_readRegister(uint16_t regNr, uint16_t *data)
    {
    uint16_t tword = 0;

    //adressnummer setzen (4 bit) und an position vor das rw-bit schieben
    tword = regNr;
    tword <<= 11;
    // rw -bit setzen(15)
    utils_set_bit_in_Word(&tword, 15, 1);
    drv_csPulse();
    HAL_SPI_TransmitReceive(&hspi1, (uint8_t*) &tword, (uint8_t*) data, 1,HAL_TIMEOUT);

    }
TD_DRV83 drv;

/*-------------update function---------------*/


