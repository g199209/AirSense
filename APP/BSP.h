/**
  **************************************************************
  * @file       BSP.h
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

#ifndef BSP_H
#define BSP_H

/* Includes */
#include <stdio.h>
#include <time.h>

#include "stm32f10x_conf.h"
#include "includes.h"

#include "WiFi.h"
#include "Sensor.h"
#include "HMI.h"
#include "RTC.h"
#include "EEPROM.h"

/** @addtogroup System Configuration
  * @brief System Configuration
  * @{
  */

/**
  * @brief  Test Point
  */
#define TPRCC      RCC_APB2Periph_GPIOA    // Test point rcc 
#define TPPort     GPIOA                   // Test Point port
#define TPPin      GPIO_Pin_8              // PA8
#define MCO_ENABLE 1u                      // MCO output enable
#define MCO_SOURCE RCC_MCO_PLLCLK_Div2     // MCO output source

#if MCO_ENABLE == 0u
#define TP_H (TPPort->BSRR = TPPin)        // Set testpoint High
#define TP_L (TPPort->BRR = TPPin)         // Set testpoint Low
#endif

/**
  * @brief  USART2 for debug use
  */
#define USART2Baud      115200
#define USART2TXPin     GPIO_Pin_2
#define USART2RXPin     GPIO_Pin_3
#define USART2IRQPRIO   15
/**
  * @brief  OS STK
  */
#define TASK_INIT_STK_SIZE      30u     // TaskInit
#define TASK_SENSOR_STK_SIZE    64u     // SensorMeasure
#define TASK_BUTTON_STK_SIZE    64u     // ButtonUpdate
#define TASK_DISPLAY_STK_SIZE   64u     // OLEDUpdate
#define TASK_WIFI_STK_SIZE      64u     // WiFiSendPacket
#define TASK_DEBUG_STK_SIZE     64u     // TaskDebug

/**
  * @brief  OS Priority
  */
#define TASK_INIT_PRIO      7u      // TaskInit
#define TASK_SENSOR_PRIO    13u     // SensorMeasure
#define TASK_BUTTON_PRIO    10u     // ButtonUpdate
#define TASK_DISPLAY_PRIO   11u     // OLEDUpdate
#define TASK_WIFI_PRIO      15u     // WiFiSendPacket
#define TASK_DEBUG_PRIO     8u      // TaskDebug


/**
  * @}
  */

/**
  * @brief  OS Events
  */
extern OS_EVENT * SemSensorDataReady;

extern volatile uint32_t SysTickCounter;

/**
  * @brief  TaskInit
  */
void TaskInit(void *p_arg);
#ifdef __DEBUG
void TaskDebug(void * p_arg);
#endif

void BSPInit(void);

#endif

