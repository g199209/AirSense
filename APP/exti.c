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
  /* ����NVIC�ж����ȼ�����:
     - 2���ر�ʾ�����ȼ�  �����ȼ��Ϸ�ȡֵΪ 0��1��2��3 
     - 2���ر�ʾ�����ȼ�  �����ȼ��Ϸ�ȡֵΪ 0��1��2��3
     - ��ֵԽ�����ȼ�Խ�ߣ�ȡֵ�����Ϸ���Χʱȡ��bitλ */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
/*==================================================================================
    NVIC_PriorityGroup   |  �����ȼ���Χ  |  �����ȼ���Χ  |   ����
  ==================================================================================
   NVIC_PriorityGroup_0  |      0         |      0-15      |   0 ���ر�ʾ�����ȼ�
                         |                |                |   4 ���ر�ʾ�����ȼ� 
  ----------------------------------------------------------------------------------
   NVIC_PriorityGroup_1  |      0-1       |      0-7       |   1 ���ر�ʾ�����ȼ�
                         |                |                |   3 ���ر�ʾ�����ȼ� 
  ----------------------------------------------------------------------------------
   NVIC_PriorityGroup_2  |      0-3       |      0-3       |   2 ���ر�ʾ�����ȼ�
                         |                |                |   2 ���ر�ʾ�����ȼ� 
  ----------------------------------------------------------------------------------
   NVIC_PriorityGroup_3  |      0-7       |      0-1       |   3 ���ر�ʾ�����ȼ�
                         |                |                |   1 ���ر�ʾ�����ȼ� 
  ----------------------------------------------------------------------------------
   NVIC_PriorityGroup_4  |      0-15      |      0         |   4 ���ر�ʾ�����ȼ�
                         |                |                |   0 ���ر�ʾ�����ȼ�   
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
	

	  /* ����AFIO��ʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB |RCC_APB2Periph_AFIO, ENABLE);
	
	   /*��ʼ��GPIOB*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;   
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;   //��ʼ��IO��Ϊ����
	  GPIO_Init(GPIOB, &GPIO_InitStructure);    
	
	  /* ��KEY1~KEY4 ������Ӧ�Ĺܽ����ӵ��ڲ��ж��� */ 
	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource12|GPIO_PinSource13|GPIO_PinSource14|GPIO_PinSource15);
      

    /* ��KEY1~KEY4��������Ϊ�ж�ģʽ�������ش����ж� */    
    EXTI_InitStructure.EXTI_Line = EXTI_Line12|EXTI_Line13|EXTI_Line14|EXTI_Line15;			   //�ж���
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;		 //�ж�ģʽ
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //�����ش���
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* ��KEY1~KEY4�������ж����ȼ�����Ϊ��� */  
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;	 //ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;   //��ռ���ȼ�Ϊ1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;		  //�����ȼ�3
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
	OSIntEnter();    
	if(EXTI_GetITStatus(EXTI_Line12)!=RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line12); //�����־λ
		OSSemPost(sem1);
	}
	
	else if(EXTI_GetITStatus(EXTI_Line13)!=RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line13); //�����־λ
		OSSemPost(sem2);
	}
	
	else if(EXTI_GetITStatus(EXTI_Line14)!=RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line14); //�����־λ
		OSSemPost(sem3);
	}
	else
	{
    EXTI_ClearITPendingBit(EXTI_Line15); //�����־λ
		OSSemPost(sem4);
	}
	OSIntExit();       
}
