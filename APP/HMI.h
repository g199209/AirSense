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

#include "BSP.h"
 
#define FLAGS_BTN1      (OS_FLAGS)0x08
#define FLAGS_BTN2      (OS_FLAGS)0x04
#define FLAGS_BTN3      (OS_FLAGS)0x02
#define FLAGS_BTN4      (OS_FLAGS)0x01
#define FLAGS_MEASURE   (OS_FLAGS)0x10
#define FLAGS_AirStart  (OS_FLAGS)0x20
#define FLAGS_AirOK     (OS_FLAGS)0x40
#define FLAGS_AirERROR  (OS_FLAGS)0x80

#define FLAG_GRP        (OS_FLAGS)(FLAGS_BTN1 | FLAGS_BTN2 | FLAGS_BTN3 | FLAGS_BTN4\
 | FLAGS_MEASURE | FLAGS_AirStart | FLAGS_AirOK | FLAGS_AirERROR)

ErrorStatus HMIInit(void);

typedef enum
{
  DisplayMeasurement,
  DisplayOverview
} DisplayContent;

void OLEDUpdate(void *p_arg);

#endif
