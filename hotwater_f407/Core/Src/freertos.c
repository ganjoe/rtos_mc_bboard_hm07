/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "terminal.h"
#include "queue.h"
#include "semphr.h"
#include "string.h"
#include "utils.h"
/*------------Motor Control-----------------*/

/*------------Propelli Base-----------------*/
#include "newCmdOrder.h"
#include "terminal.h"
#include "dbase.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
typedef StaticQueue_t osStaticMessageQDef_t;
typedef StaticSemaphore_t osStaticSemaphoreDef_t;
/* USER CODE BEGIN PTD */
extern void term_lol_sendQueue(osMessageQueueId_t myTxQueueHandle);
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
//uint8_t lReceivedValue[1];
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for myRxTask */
osThreadId_t myRxTaskHandle;
const osThreadAttr_t myRxTask_attributes = {
  .name = "myRxTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for myTxTask */
osThreadId_t myTxTaskHandle;
const osThreadAttr_t myTxTask_attributes = {
  .name = "myTxTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for myCmdTask */
osThreadId_t myCmdTaskHandle;
const osThreadAttr_t myCmdTask_attributes = {
  .name = "myCmdTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for myLogUartTask */
osThreadId_t myLogUartTaskHandle;
uint32_t myLogUartTaskBuffer[ 128 ];
osStaticThreadDef_t myLogUartTaskControlBlock;
const osThreadAttr_t myLogUartTask_attributes = {
  .name = "myLogUartTask",
  .cb_mem = &myLogUartTaskControlBlock,
  .cb_size = sizeof(myLogUartTaskControlBlock),
  .stack_mem = &myLogUartTaskBuffer[0],
  .stack_size = sizeof(myLogUartTaskBuffer),
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for myMcTask */
osThreadId_t myMcTaskHandle;
uint32_t myMcTaskBuffer[ 128 ];
osStaticThreadDef_t myMcTaskControlBlock;
const osThreadAttr_t myMcTask_attributes = {
  .name = "myMcTask",
  .cb_mem = &myMcTaskControlBlock,
  .cb_size = sizeof(myMcTaskControlBlock),
  .stack_mem = &myMcTaskBuffer[0],
  .stack_size = sizeof(myMcTaskBuffer),
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for myTxQueue */
osMessageQueueId_t myTxQueueHandle;
uint8_t myTxQueueBuffer[ 128 * sizeof( uint8_t ) ];
osStaticMessageQDef_t myTxQueueControlBlock;
const osMessageQueueAttr_t myTxQueue_attributes = {
  .name = "myTxQueue",
  .cb_mem = &myTxQueueControlBlock,
  .cb_size = sizeof(myTxQueueControlBlock),
  .mq_mem = &myTxQueueBuffer,
  .mq_size = sizeof(myTxQueueBuffer)
};
/* Definitions for myRxQueue */
osMessageQueueId_t myRxQueueHandle;
uint8_t myRxQueueBuffer[ 64 * sizeof( uint8_t ) ];
osStaticMessageQDef_t myRxQueueControlBlock;
const osMessageQueueAttr_t myRxQueue_attributes = {
  .name = "myRxQueue",
  .cb_mem = &myRxQueueControlBlock,
  .cb_size = sizeof(myRxQueueControlBlock),
  .mq_mem = &myRxQueueBuffer,
  .mq_size = sizeof(myRxQueueBuffer)
};
/* Definitions for myLineObjQueue */
osMessageQueueId_t myLineObjQueueHandle;
uint8_t myLineObjQueueBuffer[ 1 * 64 ];
osStaticMessageQDef_t myLineObjQueueControlBlock;
const osMessageQueueAttr_t myLineObjQueue_attributes = {
  .name = "myLineObjQueue",
  .cb_mem = &myLineObjQueueControlBlock,
  .cb_size = sizeof(myLineObjQueueControlBlock),
  .mq_mem = &myLineObjQueueBuffer,
  .mq_size = sizeof(myLineObjQueueBuffer)
};
/* Definitions for myCmdLineObjQueue */
osMessageQueueId_t myCmdLineObjQueueHandle;
uint8_t myCmdLineObjQueueBuffer[ 1 * 64 ];
osStaticMessageQDef_t myCmdLineObjQueueControlBlock;
const osMessageQueueAttr_t myCmdLineObjQueue_attributes = {
  .name = "myCmdLineObjQueue",
  .cb_mem = &myCmdLineObjQueueControlBlock,
  .cb_size = sizeof(myCmdLineObjQueueControlBlock),
  .mq_mem = &myCmdLineObjQueueBuffer,
  .mq_size = sizeof(myCmdLineObjQueueBuffer)
};
/* Definitions for myLogLineObjQueue */
osMessageQueueId_t myLogLineObjQueueHandle;
uint8_t myLogLineObjQueueBuffer[ 16 * 64 ];
osStaticMessageQDef_t myLogLineObjQueueControlBlock;
const osMessageQueueAttr_t myLogLineObjQueue_attributes = {
  .name = "myLogLineObjQueue",
  .cb_mem = &myLogLineObjQueueControlBlock,
  .cb_size = sizeof(myLogLineObjQueueControlBlock),
  .mq_mem = &myLogLineObjQueueBuffer,
  .mq_size = sizeof(myLogLineObjQueueBuffer)
};
/* Definitions for mySDwriteBufferLineObjQueue */
osMessageQueueId_t mySDwriteBufferLineObjQueueHandle;
uint8_t mySDwriteBufferLineObjQueueBuffer[ 8 * 64 ];
osStaticMessageQDef_t mySDwriteBufferLineObjQueueControlBlock;
const osMessageQueueAttr_t mySDwriteBufferLineObjQueue_attributes = {
  .name = "mySDwriteBufferLineObjQueue",
  .cb_mem = &mySDwriteBufferLineObjQueueControlBlock,
  .cb_size = sizeof(mySDwriteBufferLineObjQueueControlBlock),
  .mq_mem = &mySDwriteBufferLineObjQueueBuffer,
  .mq_size = sizeof(mySDwriteBufferLineObjQueueBuffer)
};
/* Definitions for myFlagNewString */
osSemaphoreId_t myFlagNewStringHandle;
osStaticSemaphoreDef_t myFlagNewStringControlBlock;
const osSemaphoreAttr_t myFlagNewString_attributes = {
  .name = "myFlagNewString",
  .cb_mem = &myFlagNewStringControlBlock,
  .cb_size = sizeof(myFlagNewStringControlBlock),
};
/* Definitions for myCountNewString */
osSemaphoreId_t myCountNewStringHandle;
osStaticSemaphoreDef_t myCountNewStringControlBlock;
const osSemaphoreAttr_t myCountNewString_attributes = {
  .name = "myCountNewString",
  .cb_mem = &myCountNewStringControlBlock,
  .cb_size = sizeof(myCountNewStringControlBlock),
};
/* Definitions for myCountNewCmd */
osSemaphoreId_t myCountNewCmdHandle;
osStaticSemaphoreDef_t myCountNewCmdControlBlock;
const osSemaphoreAttr_t myCountNewCmd_attributes = {
  .name = "myCountNewCmd",
  .cb_mem = &myCountNewCmdControlBlock,
  .cb_size = sizeof(myCountNewCmdControlBlock),
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
extern void StartRxTask(void *argument);
extern void StartTxTask(void *argument);
extern void StartCmdTask(void *argument);
void StartLogUartTask(void *argument);
extern void StartMcTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void configureTimerForRunTimeStats(void);
unsigned long getRunTimeCounterValue(void);
void vApplicationIdleHook(void);
void vApplicationTickHook(void);
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName);
void vApplicationMallocFailedHook(void);
void vApplicationDaemonTaskStartupHook(void);

/* USER CODE BEGIN 1 */
/* Functions needed when configGENERATE_RUN_TIME_STATS is on */
__weak void configureTimerForRunTimeStats(void)
    {

    }

__weak unsigned long getRunTimeCounterValue(void)
    {
    return 0;
    }
/* USER CODE END 1 */

/* USER CODE BEGIN 2 */
void vApplicationIdleHook(void)
    {
    /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
     to 1 in FreeRTOSConfig.h. It will be called on each iteration of the idle
     task. It is essential that code added to this hook function never attempts
     to block in any way (for example, call xQueueReceive() with a block time
     specified, or call vTaskDelay()). If the application makes use of the
     vTaskDelete() API function (as this demo application does) then it is also
     important that vApplicationIdleHook() is permitted to return to its calling
     function, because it is the responsibility of the idle task to clean up
     memory allocated by the kernel to any task that has since been deleted. */
    }
/* USER CODE END 2 */

/* USER CODE BEGIN 3 */
void vApplicationTickHook(void)
    {
    /* This function will be called by each tick interrupt if
     configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h. User code can be
     added here, but the tick hook is called from an interrupt context, so
     code must not attempt to block, and only the interrupt safe FreeRTOS API
     functions can be used (those that end in FromISR()). */
    }
/* USER CODE END 3 */

/* USER CODE BEGIN 4 */
void vApplicationStackOverflowHook(xTaskHandle xTask, signed char *pcTaskName)
    {
    /* Run time stack overflow checking is performed if
     configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function is
     called if a stack overflow is detected. */
    }
/* USER CODE END 4 */

/* USER CODE BEGIN 5 */
void vApplicationMallocFailedHook(void)
    {
    /* vApplicationMallocFailedHook() will only be called if
     configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h. It is a hook
     function that will get called if a call to pvPortMalloc() fails.
     pvPortMalloc() is called internally by the kernel whenever a task, queue,
     timer or semaphore is created. It is also called by various parts of the
     demo application. If heap_1.c or heap_2.c are used, then the size of the
     heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
     FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
     to query the size of free heap space that remains (although it does not
     provide information on how the remaining heap might be fragmented). */
    }
/* USER CODE END 5 */

/* USER CODE BEGIN DAEMON_TASK_STARTUP_HOOK */
void vApplicationDaemonTaskStartupHook(void)
    {
    }
/* USER CODE END DAEMON_TASK_STARTUP_HOOK */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of myFlagNewString */
  myFlagNewStringHandle = osSemaphoreNew(1, 1, &myFlagNewString_attributes);

  /* creation of myCountNewString */
  myCountNewStringHandle = osSemaphoreNew(16, 16, &myCountNewString_attributes);

  /* creation of myCountNewCmd */
  myCountNewCmdHandle = osSemaphoreNew(16, 16, &myCountNewCmd_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of myTxQueue */
  myTxQueueHandle = osMessageQueueNew (128, sizeof(uint8_t), &myTxQueue_attributes);

  /* creation of myRxQueue */
  myRxQueueHandle = osMessageQueueNew (64, sizeof(uint8_t), &myRxQueue_attributes);

  /* creation of myLineObjQueue */
  myLineObjQueueHandle = osMessageQueueNew (1, 64, &myLineObjQueue_attributes);

  /* creation of myCmdLineObjQueue */
  myCmdLineObjQueueHandle = osMessageQueueNew (1, 64, &myCmdLineObjQueue_attributes);

  /* creation of myLogLineObjQueue */
  myLogLineObjQueueHandle = osMessageQueueNew (16, 64, &myLogLineObjQueue_attributes);

  /* creation of mySDwriteBufferLineObjQueue */
  mySDwriteBufferLineObjQueueHandle = osMessageQueueNew (8, 64, &mySDwriteBufferLineObjQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of myRxTask */
  myRxTaskHandle = osThreadNew(StartRxTask, NULL, &myRxTask_attributes);

  /* creation of myTxTask */
  myTxTaskHandle = osThreadNew(StartTxTask, NULL, &myTxTask_attributes);

  /* creation of myCmdTask */
  myCmdTaskHandle = osThreadNew(StartCmdTask, NULL, &myCmdTask_attributes);

  /* creation of myLogUartTask */
  myLogUartTaskHandle = osThreadNew(StartLogUartTask, NULL, &myLogUartTask_attributes);

  /* creation of myMcTask */
  myMcTaskHandle = osThreadNew(StartMcTask, NULL, &myMcTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
    /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
    /* Infinite loop*/
    for (;;)
	{
//	qprintf(myTxQueueHandle, "asdf");
	osDelay(100);
	 HAL_UART_Receive_DMA(&huart1, (uint8_t*) &readbyte, 1);
/*
 *
	int tdsize = sizeof(TD_LINEOBJ);
	TD_LINEOBJ oneline;
	TD_LINEOBJ loadline;
	tdsize = sizeof(oneline);

	term_makeLineObj(&oneline,"cmd","setdate 9 12 80",0,0,0,0);	//not used

	term_lol_StorLineObj(myLineObjQueueHandle, &oneline);//xQueueSend(myLineObjQueueHandle, &oneline, 10);

	term_lol_LoadLineObj(myLineObjQueueHandle, &loadline);//xQueueReceive(myLineObjQueueHandle, &loadline,  ( portTickType ) 10);

	term_vprintLineObj(myTxQueueHandle, &loadline);
 */

//


	}
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartLogUartTask */
/**
* @brief Function implementing the myLogUartTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartLogUartTask */
void StartLogUartTask(void *argument)
{
  /* USER CODE BEGIN StartLogUartTask */
  /* Infinite loop */
  osStatus_t val;
  for(;;)
  {
    TD_LINEOBJ line = {0};

   val = osMessageQueueGet(myLogLineObjQueueHandle, &line, NULL, osWaitForever) ;

   switch(val)
       {
   case osOK:
       term_vprintLineObj(myTxQueueHandle, &line);
       //dbase_StoreSD( &line);
       }
  }
  /* USER CODE END StartLogUartTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
