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

#include "stm32f10x_conf.h"
#include "includes.h"

#include "WiFi.h"
#include "Sensor.h"
#include "HMI.h"

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
#define USART2TXPin     GPIO_Pin_2
#define USART2RXPin     GPIO_Pin_3
#define USART2IRQPRIO   15
/**
  * @brief  OS STK
  */
// TaskInit
#define TASK_INIT_STK_SIZE  30u

/**
  * @brief  OS Priority
  */
// TaskInit
#define TASK_INIT_PRIO      9u


/**
  * @}
  */

/**
  * @brief  TaskInit
  */
extern OS_STK Task_Init_STK[TASK_INIT_STK_SIZE];
void TaskInit(void *p_arg);


void BSPInit(void);



#endif

