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
#include "BSP.h"

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
	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource12);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource13);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource15);
      

    /* 将KEY1~KEY4按键配置为中断模式，上升沿触发中断 */    
    EXTI_InitStructure.EXTI_Line = EXTI_Line12|EXTI_Line13|EXTI_Line14|EXTI_Line15;			   //中断线
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;		 //中断模式
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //下降沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* 将KEY1~KEY4按键的中断优先级配置为最低 */  
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;	 //使能按键所在的外部中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = EXTIIRQPRIO;   //抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;		  //子优先级
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
	INT8U err;
	OSIntEnter();
  if (EXTI->PR & EXTI_Line12)
  {
    EXTI->PR = EXTI_Line12;
    OSFlagPost(Sem_Display, FLAGS_BTN4, OS_FLAG_SET, &err);
  }
  else if (EXTI->PR & EXTI_Line13)
  {
    EXTI->PR = EXTI_Line13;
    OSSemPost(SemAirKiss);
    // OSFlagPost(Sem_Display, FLAGS_BTN3, OS_FLAG_SET, &err);
  }
  else if (EXTI->PR & EXTI_Line14)
  {
    EXTI->PR = EXTI_Line14;
    OSFlagPost(Sem_Display, FLAGS_BTN2, OS_FLAG_SET, &err);
  }
  else if (EXTI->PR & EXTI_Line15)
  {
    EXTI->PR = EXTI_Line15;
    OSFlagPost(Sem_Display, FLAGS_BTN1, OS_FLAG_SET, &err);
  }
	OSIntExit();       
}
