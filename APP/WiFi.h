/**
  **************************************************************
  * @file       WiFi.h
  * @author     高明飞
  * @version    V0.1
  * @date       2016-4-3
  *
  * @brief      WiFi模块驱动
  *
  * @details 
  * @verbatim
  * WiFi模块HAL层程序头文件
  *
  * 修改记录：
  * 2016-4-3 :
  *   - File Created.
  *
  * @endverbatim
  ***************************************************************
  */

#ifndef WIFI_H
#define WIFI_H

void WiFiInit(void);

void WiFiSendPacket(void *p_arg);

#endif
