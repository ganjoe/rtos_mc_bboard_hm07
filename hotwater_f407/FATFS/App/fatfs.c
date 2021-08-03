/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

#include "fatfs.h"

uint8_t retSD;    /* Return value for SD */
char SDPath[4];   /* SD logical drive path */
FATFS SDFatFS;    /* File system object for SD logical drive */
FIL SDFile;       /* File object for SD */

/* USER CODE BEGIN Variables */

/* USER CODE END Variables */

void MX_FATFS_Init(void)
{
  /*## FatFS: Link the SD driver ###########################*/
  retSD = FATFS_LinkDriver(&SD_Driver, SDPath);

  /* USER CODE BEGIN Init */
  /* additional user code for init */
  FRESULT fres=0;
  fres = 	f_mount(&SDFatFS, SDPath, 1);
  fres =	f_open(&SDFile, "asdf", FA_READ| FA_WRITE);
  /* USER CODE END Init */
}

/**
  * @brief  Gets Time from RTC
  * @param  None
  * @retval Time in DWORD
  */
DWORD get_fattime(void)
{
  /* USER CODE BEGIN get_fattime */
  return 0;
  /* USER CODE END get_fattime */
}

/* USER CODE BEGIN Application */
int sd_lol_writeline(char* filename, char* linebuff, uint8_t chars, uint8_t line)
{
	int bytesWrote = -1;
	FRESULT fres;
	int slen;
	char* linebuffer;
/*---------------------------------------------------------*/
	linebuffer = calloc(chars, 1);	// + \r und platz für die letzte \0 von calloc
	strcpy(linebuffer, linebuff);
	memcpy(linebuffer+chars-1, "\r",1);

	slen =strlen(linebuffer);
	if (slen > chars)
		{
		linebuffer =strdup("to boku");
		}

/*---------------------------------------------------------*/
	fres = 	f_mount(&SDFatFS, SDPath, 1);
	fres =	f_open(&SDFile, filename, FA_READ| FA_WRITE);
	if (fres == FR_NO_FILE)
		 {
		 fres =	f_open(&SDFile, filename, FA_CREATE_ALWAYS | FA_WRITE | FA_READ);
		 }
	if (fres == FR_DISK_ERR)
		 {
		bytesWrote = -1;
		 }
	if (fres == FR_INVALID_DRIVE)
		 {
		bytesWrote = -1;
		 }
	if (fres == FR_OK)
		{
		 f_lseek(&SDFile, chars * line);
		 f_write(&SDFile, linebuffer, chars, &bytesWrote);
		 f_close(&SDFile);
		 f_mount(&SDFatFS, SDPath, 0);
		}


/*---------------------------------------------------------*/
	 free (linebuffer);
		return bytesWrote;
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
