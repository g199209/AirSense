/**
  **************************************************************
  * @file       RTC.h
  * @author     高明飞
  * @version    V0.1
  * @date       2016-4-4
  *
  * @brief      RTC module
  *
  * @details 
  * @verbatim
  * RTC模块及时间处理
  *
  * 修改记录：
  * 2016-4-4 :
  *   - File Created.
  *
  * @endverbatim
  ***************************************************************
  */

#ifndef RTC_H
#define RTC_H

void RTCInit(void);

//根据检测到串口输入的数据，将检测到的数据转换为对应时间单位：秒
uint32_t Time_Regulate(void);

//根綯ime_Regulate返回的数值设定RTC的计数寄存器CNTH和CNTL
void Time_Adjust(void);

//通过串口检测输入要设定的时间
uint8_t USART_Scanf(uint32_t value);

#endif
