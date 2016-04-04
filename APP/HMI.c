/**
  **************************************************************
  * @file       HMI.c
  * @author     高明飞
  * @version    V0.1
  * @date       2016-4-3
  *
  * @brief      HMI模块驱动
  *
  * @details 
  * @verbatim
  * HMI模块HAL层程序源文件
  *
  * 包括OLED屏幕及触摸按键
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
  * @brief  HMI module initialization
  *
  * @param  None
  *
  * @retval ERROR: Initialization failed
  * @retval SUCCESS: Initialization successed
  */
ErrorStatus HMIInit(void)
{
  return SUCCESS;
}

/**
  * @brief  OLED display update task
  *
  * @param  p_arg: Unused
  *
  * @retval None
  */
void OLEDUpdate(void *p_arg)
{

  while (1);
}

/**
  * @brief  Button update task
  *
  * @param  p_arg: Unused
  *
  * @retval None
  */
void ButtonUpdate(void *p_arg)
{

  while (1);
}
