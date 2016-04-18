/**
  **************************************************************
  * @file       BSP.c
  * @author     高明飞
  * @version    V0.1
  * @date       2016-4-3
  *
  * @brief      Board Support Package
  *
  * @details 
  * @verbatim
  * 板级支持包
  *
  * 修改记录：
  * 2016-4-1 :
  *   - File Created.
  *
  * @endverbatim
  ***************************************************************
  */

#include "BSP.h"

/**
  * @brief  OS STK
  */
OS_STK Task_Init_STK[TASK_INIT_STK_SIZE];
OS_STK Task_Sensor_STK[TASK_SENSOR_STK_SIZE];
OS_STK Task_Airkiss_STK[TASK_AIRKISS_STK_SIZE];
OS_STK Task_Display_STK[TASK_DISPLAY_STK_SIZE];
OS_STK Task_WiFi_STK[TASK_WIFI_STK_SIZE];
#ifdef __DEBUG
OS_STK Task_Debug_STK[TASK_DEBUG_STK_SIZE];
#endif

/**
  * @brief  OS Event
  */
OS_EVENT * SemSensorDataReady;
OS_EVENT * SemAirKiss;
OS_FLAG_GRP * Sem_Display;

/**
  * @brief  SysTick Counter
  */
volatile uint32_t SysTickCounter;

/**
  * @brief  Private function prototype
  */
static void DebugInit(void);

volatile uint8_t RXdata;

/**
  * @brief  BSP initialization
  *
  * @param  None
  *
  * @retval None
  */
void BSPInit(void)
{
  /* Initialize System Tick Timer & NVIC*/
  SysTick_Config(SystemCoreClock / OS_TICKS_PER_SEC);
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

  /* Debug function initialization */
  DebugInit();

  /* RTC initialization */
  RTCInit();

#ifdef __DEBUG
  printf("\r\n\r\n\r\n");
  printf("$$$$$$$$$$$$$$$$$$$$$$$$\r\n");
  printf("~~~~ Program Start! ~~~~\r\n");
  printf("$$$$$$$$$$$$$$$$$$$$$$$$\r\n");
  printf("\r\n");
  printf("\r\nDebug interface initialize OK!\r\n");
#endif

  /* Sensor module initialization */
#ifdef __DEBUG
  if (SensorInit() == SUCCESS)
    printf("\r\nSensor initialize OK!\r\n");
  else
    printf("\r\nSensor initialize FAILED!\r\n");
#else
  SensorInit();
#endif

  /* HMI module initialization */
#ifdef __DEBUG
  if (HMIInit() == SUCCESS)
    printf("\r\nHMI initialize OK!\r\n");
  else
    printf("\r\nHMI initialize FAILED!\r\n");
#else
  HMIInit();
#endif

  /* WiFi module initialization */
#ifdef __DEBUG
  if (WiFiInit() == SUCCESS)
    printf("\r\nWiFi initialize OK!\r\n");
  else
    printf("\r\nWiFi initialize FAILED!\r\n");
#else
  WiFiInit();
#endif

  /* EEPROM initialization */
#ifdef __DEBUG
  if (EEPROMInit() == SUCCESS)
    printf("\r\nEEPROM initialize OK!\r\n");
  else
    printf("\r\nEEPROM initialize FAILED!\r\n");
#else
  EEPROMInit();
#endif
}

/**
  * @brief  Initialization task, used to create other tasks
  *
  * @param  p_arg: Unused
  *
  * @retval None
  */
void TaskInit(void *p_arg)
{
	INT8U err;
  INT8U result = 0u;

  OSStatInit();
  
  /* Create Tasks */
  result += OSTaskCreate(SensorMeasure, (void *)0, &Task_Sensor_STK[TASK_SENSOR_STK_SIZE - 1], TASK_SENSOR_PRIO);
  result += OSTaskCreate(Airkiss, (void *)0, &Task_Airkiss_STK[TASK_AIRKISS_STK_SIZE - 1], TASK_AIRKISS_PRIO);
  result += OSTaskCreate(OLEDUpdate, (void *)0, &Task_Display_STK[TASK_DISPLAY_STK_SIZE - 1], TASK_DISPLAY_PRIO);
  result += OSTaskCreate(WiFiSendPacket, (void *)0, &Task_WiFi_STK[TASK_WIFI_STK_SIZE - 1], TASK_WIFI_PRIO);
#ifdef __DEBUG
  result += OSTaskCreate(TaskDebug, (void *)0, &Task_Debug_STK[TASK_DEBUG_STK_SIZE - 1], TASK_DEBUG_PRIO);
  if (result == 0u)
    printf("Task Create OK!\r\n");
  else
    printf("Task Create FAILED!\r\n");
#endif

  /* Create Sems */
  SemSensorDataReady = OSSemCreate(0);
  Sem_Display = OSFlagCreate(0, &err);
  SemAirKiss = OSSemCreate(0);

  OSTaskDel(OS_PRIO_SELF);
}

/**
  * @brief  Debug task
  *
  * @param  p_arg: Unused
  *
  * @retval None
  */
#ifdef __DEBUG
void TaskDebug(void * p_arg)
{
  OSTaskSuspend(OS_PRIO_SELF);

  //time_t t;
  //t = time(0);
  //while (1)
  //{
  //  // printf("Time = %d\r\n", t);
  //  //OSSemPost(SensorDataReady);
  //  //OSTimeDlyHMSM(0, 0, 5, 0);
  //}
}
#endif

/**
  * @brief  Debug function initialization
  *
  * @param  None
  *
  * @retval None
  */
static void DebugInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Test Point */
  RCC_APB2PeriphClockCmd(TPRCC, ENABLE);

  GPIO_InitStructure.GPIO_Pin = TPPin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
#if MCO_ENABLE == 1
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  RCC_MCOConfig(MCO_SOURCE);
#else
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
#endif
  GPIO_Init(TPPort, &GPIO_InitStructure);

  /* USART2 for debug use */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  GPIO_InitStructure.GPIO_Pin = USART2TXPin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = USART2RXPin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* 115200 bps, 8 bit + 1 stop bit */
  USART_InitStructure.USART_BaudRate = USART2Baud;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_Init(USART2, &USART_InitStructure);

  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART2IRQPRIO;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_Init(&NVIC_InitStructure);

  USART_Cmd(USART2, ENABLE);
}

/**
  * @brief  This function handles SysTick Handler.
  *
  * @detail Frequency depends on OS_TICKS_PER_SEC
  *
  * @param  None
  *
  * @retval None
  */
void SysTick_Handler(void)
{
  OSIntEnter();

  SysTickCounter++;
  OSTimeTick();   // 调用uC/OS时钟服务程序

  OSIntExit();
}

/**
  * @brief  This function handles USART2 RXNE Handler.
  *
  * @param  None
  *
  * @retval None
  */
void USART2_IRQHandler(void)
{
  OSIntEnter();
  //if ((USART2->SR & USART_SR_RXNE) != 0x00)
  //
  RXdata = USART2->DR & 0xFF;
  USART1->DR = USART2->DR;
  OSIntExit();
}


/**
  * @brief  Retargets the C library printf function to the USART.
  *
  * @param  ch: the char to be send.
  * @param  *f: 
  *
  * @retval int: the char that send out.
  */
int fputc(int ch, FILE *f)
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */

  /* Loop until the end of transmission */
  while ((USART2->SR & USART_SR_TC) == 0);
  USART2->DR = (uint8_t)ch;

  return ch;
}

/**
  * @brief  Retargets the C library printf function to the USART.
  *
  * @param  *f
  *
  * @retval int: the char that received.
  */
int fgetc(FILE *f)
{
  int ch;
  while ((USART2->SR & USART_SR_RXNE) == 0);
  ch = USART2->DR & 0xFF;

  return ch;
}

