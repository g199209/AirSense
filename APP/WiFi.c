/**
  **************************************************************
  * @file       WiFi.c
  * @author     ������
  * @version    V0.1
  * @date       2016-4-3
  *
  * @brief      WiFiģ������
  *
  * @details 
  * @verbatim
  * WiFiģ��HAL�����Դ�ļ�
  *
  * �޸ļ�¼��
  * 2016-4-3 :
  *   - File Created.
  *
  * @endverbatim
  ***************************************************************
  */

#include "BSP.h"

/* Private functions */
static void WiFiOSTaskInit(void);

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
  
  return SUCCESS;
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
  while (1)
  {
    OSSemPend(SensorDataReady, 0, &err);
    printf("WiFi\r\n");
  }
}


/**
  * @brief  Initialize WiFi OS Task
  *
  * @param  None
  *
  * @retval None
  */
static void WiFiOSTaskInit(void)
{
  /* Create */
  

}
