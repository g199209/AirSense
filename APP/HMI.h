/**
  **************************************************************
  * @file       HMI.h
  * @author     高明飞
  * @version    V0.1
  * @date       2016-4-3
  *
  * @brief      HMI模块驱动
  *
  * @details 
  * @verbatim
  * HMI模块HAL层程序头文件
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

#ifndef HMI_H
#define HMI_H

#include "spi.h"
#include "oled.h"
#include "exti.h"
#include "ucos_ii.h"
 


ErrorStatus HMIInit(void);

void OLEDUpdate(void *p_arg);
void ButtonUpdate(void *p_arg);

#endif
