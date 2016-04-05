/**
  **************************************************************
  * @file       exti.c
  * @author     ldliu
  * @version    V0.1
  * @date       2016-4-1
  *
  * @brief      按键中断模块
  *
  * @details 
  * @verbatim
  * 
  *
  * @endverbatim
  ***************************************************************
  */

#include "exti.h"
#include "oled.h"
#include "ucos_ii.h"
#include "HMI.h"
/**
  * @brief  NVIC_GroupConfig function
  *
  * @param  None
  *
  * @retval None
  */
void NVIC_GroupConfig(void)
{
  /* 配置NVIC中断优先级分组:
     - 2比特表示主优先级  主优先级合法取值为 0、1、2、3 
     - 2比特表示次优先级  次优先级合法取值为 0、1、2、3
     - 数值越低优先级越高，取值超过合法范围时取低bit位 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
/*==================================================================================
    NVIC_PriorityGroup   |  主优先级范围  |  次优先级范围  |   描述
  ==================================================================================
   NVIC_PriorityGroup_0  |      0         |      0-15      |   0 比特表示主优先级
                         |                |                |   4 比特表示次优先级 
  ----------------------------------------------------------------------------------
   NVIC_PriorityGroup_1  |      0-1       |      0-7       |   1 比特表示主优先级
                         |                |                |   3 比特表示次优先级 
  ----------------------------------------------------------------------------------
   NVIC_PriorityGroup_2  |      0-3       |      0-3       |   2 比特表示主优先级
                         |                |                |   2 比特表示次优先级 
  ----------------------------------------------------------------------------------
   NVIC_PriorityGroup_3  |      0-7       |      0-1       |   3 比特表示主优先级
                         |                |                |   1 比特表示次优先级 
  ----------------------------------------------------------------------------------
   NVIC_PriorityGroup_4  |      0-15      |      0         |   4 比特表示主优先级
                         |                |                |   0 比特表示次优先级   
  ==================================================================================*/
}

/**
  * @brief  EXTIX_Init function initialization
  *
  * @param  None
  *
  * @retval None
  */
void EXTIX_Init(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure; 
 	  EXTI_InitTypeDef EXTI_InitStructure;
   	NVIC_InitTypeDef NVIC_InitStructure;
	

	  /* 开启AFIO的时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB |RCC_APB2Periph_AFIO, ENABLE);
	
	   /*初始化GPIOB*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;   
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //初始化IO口为输入
	  GPIO_Init(GPIOB, &GPIO_InitStructure);    
	
	  /* 将KEY1~KEY4 按键对应的管脚连接到内部中断线 */ 
	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource12|GPIO_PinSource13|GPIO_PinSource14|GPIO_PinSource15);
      

    /* 将KEY1~KEY4按键配置为中断模式，上升沿触发中断 */    
    EXTI_InitStructure.EXTI_Line = EXTI_Line12|EXTI_Line13|EXTI_Line14|EXTI_Line15;			   //中断线
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;		 //中断模式
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //上升沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* 将KEY1~KEY4按键的中断优先级配置为最低 */  
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;	 //使能按键所在的外部中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;   //抢占优先级为1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;		  //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);				 //根据NVIC_InitStructure中指定的参数初始化外设NVIC

	
}

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
		EXTI_ClearITPendingBit(EXTI_Line12); //清楚标志位
		OSSemPost(sem1);
	}
	
	else if(EXTI_GetITStatus(EXTI_Line13)!=RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line13); //清楚标志位
		OSSemPost(sem2);
	}
	
	else if(EXTI_GetITStatus(EXTI_Line14)!=RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line14); //清楚标志位
		OSSemPost(sem3);
	}
	else
	{
    EXTI_ClearITPendingBit(EXTI_Line15); //清楚标志位
		OSSemPost(sem4);
	}
	OSIntExit();       
}
