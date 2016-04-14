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

/* Pin define */
#define WIFIRXPIN       GPIO_Pin_10
#define WIFITXPIN       GPIO_Pin_9
#define WIFIENPIN       GPIO_Pin_11
#define WIFIRSTPIN      GPIO_Pin_12

/* UASRT define */
#define WIFIUART        USART1
#define WIFIBAUDRATE    115200
#define WIFIIRQPRIO     13

/* UART Buffer Size */
#define WIFIBUFFERSIZE  1024

extern volatile uint8_t TCP_Connected;

ErrorStatus WiFiInit(void);
ErrorStatus WiFiAirKiss(uint32_t timeout);
ErrorStatus WiFiCreateSocket(void);

void WiFiSendPacket(void *p_arg);

#endif
