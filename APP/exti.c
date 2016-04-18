/**
  **************************************************************
  * @file       exti.c
  * @author     ldliu
  * @version    V0.1
  * @date       2016-4-1
  *
  * @brief      �����ж�ģ��
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
	

	  /* ����AFIO��ʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB |RCC_APB2Periph_AFIO, ENABLE);
	
	   /*��ʼ��GPIOB*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;   
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //��ʼ��IO��Ϊ����
	  GPIO_Init(GPIOB, &GPIO_InitStructure);    
	
	  /* ��KEY1~KEY4 ������Ӧ�Ĺܽ����ӵ��ڲ��ж��� */ 
	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource12);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource13);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource15);
      

    /* ��KEY1~KEY4��������Ϊ�ж�ģʽ�������ش����ж� */    
    EXTI_InitStructure.EXTI_Line = EXTI_Line12|EXTI_Line13|EXTI_Line14|EXTI_Line15;			   //�ж���
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;		 //�ж�ģʽ
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //�½��ش���
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* ��KEY1~KEY4�������ж����ȼ�����Ϊ��� */  
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;	 //ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = EXTIIRQPRIO;   //��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;		  //�����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //ʹ���ⲿ�ж�ͨ��
    NVIC_Init(&NVIC_InitStructure);				 //����NVIC_InitStructure��ָ���Ĳ�����ʼ������NVIC
}

/**-------------------------------------------------------
  * @������ EXTI15_10_IRQHandler
  * @����   �����жϷ�������
  * @����   ��
  * @����ֵ ��
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
