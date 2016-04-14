/**
  **************************************************************
  * @file       WiFi.c
  * @author     高明飞
  * @version    V0.1
  * @date       2016-04-12
  *
  * @brief      WiFi模块驱动
  *
  * @details 
  * @verbatim
  * WiFi模块HAL层程序源文件
  *
  * 修改记录：
  * 2016-4-3 :
  *   - File Created.
  *
  * @endverbatim
  ***************************************************************
  */

#include "BSP.h"

/* AT Suffix */
const char AT_Suffix[] = "\r\n";

/* AT Command */
const char AT_AT[] = "AT";
const char AT_CWLAP[] = "AT+CWLAP";
const char AT_Airkiss[] = "AT+CWSTARTSMART";
const char AT_AIrkiss_Release[] = "AT+CWSTOPSMART";
const char AT_CWMODE[] = "AT+CWMODE_CUR=1";
const char AT_TCP[] = "AT+CIPSTART=\"TCP\",\"115.159.183.229\",6666";
const char AT_CIPMODE[] = "AT+CIPMODE=1";
const char AT_CIPSEND[] = "AT+CIPSEND";

/* AT Response */
const char AT_OK[] = "OK";
const char AT_AirkissConnected[28] = "smartconfig connected wifi\r\n";

volatile uint8_t TCP_Connected = 0;

/* Rx Buffer */
volatile uint32_t Buffer_Count;       // Index
volatile char Rx_Buffer[WIFIBUFFERSIZE];

#define EnableWiFiRx()    (WIFIUART->CR1 |= USART_CR1_RE)
#define DisableWiFiRx()   (WIFIUART->CR1 &= ~USART_CR1_RE)

/**
  * @brief  Buffer Error
  */
enum {BufferReady, BufferOverFlow} WiFiBufferError;

/* Private functions */
static void WiFiRST(void);
static void ATSend(const char * CMD);             // Send AT Command
static ErrorStatus ATCheck(uint32_t timeout);     // Check AT Command Response

/**
  * @brief  AT Response state machine
  */
typedef enum 
{
  FindCMDStart,
  FindCMDEnd,
  FindOK
} ATResState;


/**
  * @brief  WiFi module initialization
  *
  * @param  None
  *
  * @retval ERROR: Initialization failed
  * @retval SUCCESS: Initialization successed
  */
ErrorStatus WiFiInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  ErrorStatus err;

  /* RCC */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  /* GPIO */
  GPIO_InitStructure.GPIO_Pin =WIFITXPIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = WIFIRXPIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = WIFIENPIN | WIFIRSTPIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* UART */
  USART_InitStructure.USART_BaudRate = WIFIBAUDRATE;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Tx;   // Do not enable Rx now
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_Init(WIFIUART, &USART_InitStructure);
  USART_ITConfig(WIFIUART, USART_IT_RXNE, ENABLE);

  /* NVIC */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = WIFIIRQPRIO;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_Init(&NVIC_InitStructure);

  USART_Cmd(WIFIUART, ENABLE);

  WiFiRST();
  /* Send AT to check wether WiFi module OK */
  ATSend(AT_AT);
  err = ATCheck(500);
  DisableWiFiRx();
  return err;
}

/**
  * @brief  WiFi send packet task
  *
  * @param  p_arg: Unused
  *
  * @retval None
  */
void WiFiSendPacket(void *p_arg)
{
  INT8U err;
  char * ptr;
  uint32_t i;

  //OSTaskSuspend(OS_PRIO_SELF);
  while (1)
  {
    OSSemPend(SemSensorDataReady, 0, &err);
    
    /* Send Data */
    if (TCP_Connected)
    {
      Buffer_Count = 0;   // Clear Buffer, otherwise the buffer may overflow!!
      #ifdef __DEBUG
      printf("\r\nSend TCP packet!\r\n");
      #endif
      SysTickCounter = 0;
      while (SysTickCounter < 5);
      for (ptr = (void *)&SensorMeasureData, i = 0; i < sizeof(SensorMeasureData); ptr++, i++)
      {
        while ((WIFIUART->SR & USART_SR_TC) == 0);
        WIFIUART->DR = *ptr;
      }
    }
    #ifdef __DEBUG
    else
      printf("\r\nSend TCP packet FAILED!  -  TCP Connected == 0\r\n");
    #endif
  }
}

/**
  * @brief  AirKiss
  *
  * @param  timeout: unit, ms
  *
  * @retval ERROR: Airkiss failed
  * @retval SUCCESS: Airkiss successed
  */
ErrorStatus WiFiAirKiss(uint32_t timeout)
{
  uint32_t i;
  uint32_t Buffer_Count_Start = 0;

  /* Station Mode */
  ATSend(AT_CWMODE);
  if (ATCheck(500) == ERROR)
  {
#ifdef __DEBUG
    printf("CWMODE FAILED!\r\n");
#endif
    return ERROR;
  }

  ATSend(AT_Airkiss);
  if (ATCheck(500) == ERROR)
  {
#ifdef __DEBUG
    printf("Enter Airkiss FAILED!\r\n");
#endif
    return ERROR;
  }
#ifdef __DEBUG
  else
  printf("\r\nStart Airkiss!\r\n");
#endif

  // Get Airkiss Progress
  /* Modify timeout value according to OS_TICKS_PER_SEC */
  timeout = OS_TICKS_PER_SEC * timeout / 1000;
  WiFiBufferError = BufferReady;
  Buffer_Count = 0;
  SysTickCounter = 0;
  
  while (SysTickCounter < timeout)
  {
    /* Check overflow */
    if (WiFiBufferError == BufferOverFlow)
    {
#ifdef __DEBUG
      printf("WiFi buffer overflow!\r\n");
#endif
      return ERROR;
    }

    if (Buffer_Count - Buffer_Count_Start >= 28)
    {
      for (i = 0; i < 28; i++)
      {
        if (Rx_Buffer[Buffer_Count_Start + i] != AT_AirkissConnected[i])
          break;
      }
      // Do not match
      if (i < 28)
        Buffer_Count_Start++;
      // Match
      else
      {
#ifdef __DEBUG
        printf("\r\nAirkiss OK!\r\n");
#endif
        /* Release */
        ATSend(AT_AIrkiss_Release);
        return ATCheck(500);
      }
    }
  }

  // Timeout
#ifdef __DEBUG
  printf("Airkiss Timeout!\r\n");
#endif
  /* Release */
  ATSend(AT_AIrkiss_Release);
  return ERROR;
}

/**
  * @brief  Create TCP socket
  *
  * @param  None
  *
  * @retval ERROR: TCP socket create failed
  * @retval SUCCESS: TCP socket create successed
  */
ErrorStatus WiFiCreateSocket(void)
{
  ATSend(AT_TCP);
  if (ATCheck(1000) == SUCCESS)
  {
    #ifdef __DEBUG
    printf("\r\nTCP Connected!\r\n");
    #endif
    ATSend(AT_CIPMODE);
    if (ATCheck(500) == SUCCESS)
    {
      ATSend(AT_CIPSEND);
      if (ATCheck(500) == SUCCESS)
      {
        #ifdef __DEBUG
        printf("\r\nnCIPMODE & CIPSEND OK!\r\n");
        return SUCCESS;
        #endif
      }
    }
    #ifdef __DEBUG
    printf("\r\nCIPMODE or CIPSEND FAILED!\r\n");
    #endif
    return ERROR;
  }
  else
  {
    #ifdef __DEBUG
    printf("\r\nTCP Socket Create FAILED!\r\n");
    #endif
    return ERROR;
  }
}

/**
  * @brief  Reset and enable WiFi module
  *
  * @param  None
  *
  * @retval None
  */
static void WiFiRST(void)
{
  /* Reset delay time, 100ms */
  uint32_t timeout = OS_TICKS_PER_SEC / 10;

  /* Reset */
  GPIO_ResetBits(GPIOA, WIFIRSTPIN);
  SysTickCounter = 0;
  while (SysTickCounter < timeout);
  GPIO_SetBits(GPIOA, WIFIRSTPIN);

  /* Enable */
  GPIO_SetBits(GPIOA, WIFIENPIN);
  
  /* Delay 1s */
  timeout = OS_TICKS_PER_SEC;
  SysTickCounter = 0;
  while (SysTickCounter < timeout);
}

/**
  * @brief  Send AT Command
  *
  * @param  CMD: AT Command
  *
  * @retval None
  */
static void ATSend(const char * CMD)
{
  const char * ptr;

  EnableWiFiRx();    // Enable Rx first.
  /* Initialize Buffer */
  WiFiBufferError = BufferReady;
  Buffer_Count = 0;

  /* Send Data */
  for (ptr = CMD; *ptr != '\0'; ptr++)
  {
    while ((WIFIUART->SR & USART_SR_TC) == 0);
    WIFIUART->DR = *ptr;
  }

  /* Send Suffix */
  for (ptr = AT_Suffix; *ptr != '\0'; ptr++)
  {
    while ((WIFIUART->SR & USART_SR_TC) == 0);
    WIFIUART->DR = *ptr;
  }
}

/**
  * @brief  Check AT command response
  *
  * @param  timeout: unit, ms
  *
  * @retval ERROR: AT commmand test failed
  * @retval SUCCESS: AT commmand test successed
  */
static ErrorStatus ATCheck(uint32_t timeout)
{
  ATResState ATState = FindCMDStart;
  uint32_t Buffer_Count_Start = 0;

  /* Modify timeout value according to OS_TICKS_PER_SEC */
  timeout = OS_TICKS_PER_SEC * timeout / 1000;

  SysTickCounter = 0;
  while (SysTickCounter < timeout)
  {
    /* Check overflow */
    if (WiFiBufferError == BufferOverFlow)
    {
#ifdef __DEBUG
      printf("WiFi buffer overflow!\r\n");
#endif
      return ERROR;
    }

    /* Get response text, use state machine */
    switch (ATState)
    {
    case FindCMDStart:
      if (Buffer_Count - Buffer_Count_Start >= 2)
      {
        if (Rx_Buffer[Buffer_Count_Start] == AT_AT[0] && Rx_Buffer[Buffer_Count_Start + 1] == AT_AT[1])
        {
          Buffer_Count_Start += 2;
          ATState = FindCMDEnd;
        }
        else
        {
          /* ERROR */
#ifdef __DEBUG
          printf("WiFi FindCMDStart Error!\r\n");
#endif
          return ERROR;
        }
      }
      break;

    case FindCMDEnd:
      if (Buffer_Count - Buffer_Count_Start >= 2)
      {
        if (Rx_Buffer[Buffer_Count_Start] == AT_Suffix[0] && Rx_Buffer[Buffer_Count_Start + 1] == AT_Suffix[1])
        {
          // Do not use Buffer_Count_Start += 2;
          ATState = FindOK;
        }
        else
          Buffer_Count_Start += 1;
      }
      break;

    case FindOK:
      if (Buffer_Count - Buffer_Count_Start >= 8)
      {
        if (Rx_Buffer[Buffer_Count_Start] == AT_Suffix[0] && Rx_Buffer[Buffer_Count_Start + 1] == AT_Suffix[1]
          && Rx_Buffer[Buffer_Count_Start + 2] == AT_Suffix[0] && Rx_Buffer[Buffer_Count_Start + 3] == AT_Suffix[1]
          && Rx_Buffer[Buffer_Count_Start + 4] == AT_OK[0] && Rx_Buffer[Buffer_Count_Start + 5] == AT_OK[1]
          && Rx_Buffer[Buffer_Count_Start + 6] == AT_Suffix[0] && Rx_Buffer[Buffer_Count_Start + 7] == AT_Suffix[1])
        {
          return SUCCESS;
        }
        else
          Buffer_Count_Start += 1;
      }
      break;
    }
    /* End of switch */

  }

  // Timeout
#ifdef __DEBUG
  printf("AT Timeout!\r\n");
#endif
  return ERROR;
}

/**
* @brief  This function handles USART2 RXNE Handler.
*
* @param  None
*
* @retval None
*/
void USART1_IRQHandler(void)
{
  OSIntEnter();

  Rx_Buffer[Buffer_Count] = (char)USART1->DR;
  if (Buffer_Count < WIFIBUFFERSIZE - 1)  // Buffer_Count < WIFIBUFFERSIZE is WRONG!!!!
    Buffer_Count++;
  // Buffer Overflow, disable Rx
  else
  {
    WiFiBufferError = BufferOverFlow;
    DisableWiFiRx();
  }

#ifdef __DEBUG
  USART2->DR = USART1->DR;
#endif

  OSIntExit();
}
