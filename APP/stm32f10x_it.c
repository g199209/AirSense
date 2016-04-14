/**
  **************************************************************
  * @file       stm32f10x_it.c
  * @author     ldliu
  * @version    V0.1
  * @date       2016-4-1
  *
  * @brief      中断服务程序
  *
  * @details 
  * @verbatim
  * 
  *
  * @endverbatim
  ***************************************************************
  */



/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "oled.h"
#include "ucos_ii.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**-------------------------------------------------------
  * @函数名 NMI_Handler
  * @功能   不可屏蔽中断的中断处理函数
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
void NMI_Handler(void)
{
}

/**-------------------------------------------------------
  * @函数名 HardFault_Handler
  * @功能   硬件错误中断的中断处理函数
  *         可添加函数，现在为空while循环，便于仿真器捕捉
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**-------------------------------------------------------
  * @函数名 MemManage_Handler
  * @功能   存储器访问错误中断的中断处理函数
  *         可添加函数，现在为空while循环，便于仿真器捕捉
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**-------------------------------------------------------
  * @函数名 BusFault_Handler
  * @功能   总线访问错误中断的中断处理函数
  *         可添加函数，现在为空while循环，便于仿真器捕捉
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**-------------------------------------------------------
  * @函数名 UsageFault_Handler
  * @功能   用法错误中断的中断处理函数
  *         可添加函数，现在为空while循环，便于仿真器捕捉
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**-------------------------------------------------------
  * @函数名 SVC_Handler
  * @功能   系统服务中断的中断处理函数
  *         一般会被操作系统使用
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
void SVC_Handler(void)
{
}

/**-------------------------------------------------------
  * @函数名 DebugMon_Handler
  * @功能   调试监控中断的中断处理函数
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
void DebugMon_Handler(void)
{
}

/**-------------------------------------------------------
  * @函数名 PendSV_Handler
  * @功能   可挂起的系统服务请求处理函数
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
/*void PendSV_Handler(void)
{
}*/

/**-------------------------------------------------------
  * @函数名 EXTI15_10_IRQHandler
  * @功能   按键中断服务处理函数
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
void EXTI15_10_IRQHandler(void)
{
	OSIntEnter();    
	if(EXTI_GetITStatus(EXTI_Line12)!=RESET)
	{
			OLED_CLS();  
	
	  OLED_DLY_ms(500);
		OLED_ShowString(8,0,"Jiang");	
	
	   OLED_Refresh_Gram();//
		EXTI_ClearITPendingBit(EXTI_Line12); //清楚标志位
	}
	
	else if(EXTI_GetITStatus(EXTI_Line13)!=RESET)
	{
			OLED_CLS();  
	
	  OLED_DLY_ms(50);
		
		OLED_ShowString(48,16,"xiao");	
			    
	   OLED_Refresh_Gram();//
		 EXTI_ClearITPendingBit(EXTI_Line13); //清楚标志位
	}
	
	else if(EXTI_GetITStatus(EXTI_Line14)!=RESET)
	{
			OLED_CLS();  
	
	  OLED_DLY_ms(500);
		
		OLED_ShowString(80,32,"qian");	
	  
	   OLED_Refresh_Gram();//
		EXTI_ClearITPendingBit(EXTI_Line14); //清楚标志位
	}
	else
	{
		OLED_CLS();  
	
	  OLED_DLY_ms(500);
		
		OLED_ShowString(103,48,"JXQ");											    
	   OLED_Refresh_Gram();//
    EXTI_ClearITPendingBit(EXTI_Line15); //清楚标志位
	}
	GPIO_SetBits(GPIOA, GPIO_Pin_8);
	OSIntExit();       
}

