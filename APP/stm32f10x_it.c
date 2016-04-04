/**
  **************************************************************
  * @file       stm32f10x_it.c
  * @author     ldliu
  * @version    V0.1
  * @date       2016-4-1
  *
  * @brief      �жϷ������
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
  * @������ NMI_Handler
  * @����   ���������жϵ��жϴ�����
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/
void NMI_Handler(void)
{
}

/**-------------------------------------------------------
  * @������ HardFault_Handler
  * @����   Ӳ�������жϵ��жϴ�����
  *         ����Ӻ���������Ϊ��whileѭ�������ڷ�������׽
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**-------------------------------------------------------
  * @������ MemManage_Handler
  * @����   �洢�����ʴ����жϵ��жϴ�����
  *         ����Ӻ���������Ϊ��whileѭ�������ڷ�������׽
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**-------------------------------------------------------
  * @������ BusFault_Handler
  * @����   ���߷��ʴ����жϵ��жϴ�����
  *         ����Ӻ���������Ϊ��whileѭ�������ڷ�������׽
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**-------------------------------------------------------
  * @������ UsageFault_Handler
  * @����   �÷������жϵ��жϴ�����
  *         ����Ӻ���������Ϊ��whileѭ�������ڷ�������׽
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**-------------------------------------------------------
  * @������ SVC_Handler
  * @����   ϵͳ�����жϵ��жϴ�����
  *         һ��ᱻ����ϵͳʹ��
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/
void SVC_Handler(void)
{
}

/**-------------------------------------------------------
  * @������ DebugMon_Handler
  * @����   ���Լ���жϵ��жϴ�����
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/
void DebugMon_Handler(void)
{
}

/**-------------------------------------------------------
  * @������ PendSV_Handler
  * @����   �ɹ����ϵͳ������������
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/
/*void PendSV_Handler(void)
{
}*/

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
			OLED_CLS();  
	
	  OLED_DLY_ms(500);
		OLED_ShowString(8,0,"Jiang");	
	
	   OLED_Refresh_Gram();//
		EXTI_ClearITPendingBit(EXTI_Line12); //�����־λ
	}
	
	else if(EXTI_GetITStatus(EXTI_Line13)!=RESET)
	{
			OLED_CLS();  
	
	  OLED_DLY_ms(50);
		
		OLED_ShowString(48,16,"xiao");	
			    
	   OLED_Refresh_Gram();//
		 EXTI_ClearITPendingBit(EXTI_Line13); //�����־λ
	}
	
	else if(EXTI_GetITStatus(EXTI_Line14)!=RESET)
	{
			OLED_CLS();  
	
	  OLED_DLY_ms(500);
		
		OLED_ShowString(80,32,"qian");	
	  
	   OLED_Refresh_Gram();//
		EXTI_ClearITPendingBit(EXTI_Line14); //�����־λ
	}
	else
	{
		OLED_CLS();  
	
	  OLED_DLY_ms(500);
		
		OLED_ShowString(103,48,"JXQ");											    
	   OLED_Refresh_Gram();//
    EXTI_ClearITPendingBit(EXTI_Line15); //�����־λ
	}
	GPIO_SetBits(GPIOA, GPIO_Pin_8);
	OSIntExit();       
}

