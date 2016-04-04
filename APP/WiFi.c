/**
  **************************************************************
  * @file       WiFi.c
  * @author     高明飞
  * @version    V0.1
  * @date       2016-4-3
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

}
