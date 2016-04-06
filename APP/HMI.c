/**
  **************************************************************
  * @file       HMI.c
  * @author     ������
  * @version    V0.1
  * @date       2016-4-3
  *
  * @brief      HMIģ������
  *
  * @details 
  * @verbatim
  * HMIģ��HAL�����Դ�ļ�
  *
  * ����OLED��Ļ����������
  *
  * �޸ļ�¼��
  * 2016-4-3 :
  *   - File Created.
  *
  * @endverbatim
  ***************************************************************
  */

#include "BSP.h"

/**
  * @brief  HMI module initialization
  *
  * @param  None
  *
  * @retval ERROR: Initialization failed
  * @retval SUCCESS: Initialization successed
  */
ErrorStatus HMIInit(void)
{
	 //GPIO_ResetBits(GPIOA, GPIO_Pin_8);
	 NVIC_GroupConfig();
   EXTIX_Init();
   SPIx_Init();
   OLED_Init();//OLED��ʼ��
	
	  //���Կ���OK
	 OLED_Display_Off();
	 OLED_Display_On();
	 OLED_CLS();  
	
	 OLED_DLY_ms(5000);
	 OLED_ShowString(8,0,"Dear");	
	 OLED_ShowString(48,0,"LiMin Lady:");	
	 OLED_Refresh_Gram();
	
  return SUCCESS;
}

/**
  * @brief  OLED display update task
  *
  * @param  p_arg: Unused
  *
  * @retval None
  */
void OLEDUpdate(void *p_arg)
{

  while (1)
  {
		//if�������ж�
		OSFlagPend(Sem_Display, (OS_FLAGS)240, OS_FLAG_WAIT_SET_ANY, 0, &err);  //����0~3λ
		// ������
		if(Sem_Display->OSFlagFlags&0x10)
		{
			OLED_DLY_ms(5000);
	    OLED_ShowString(8,0,"Dear");	
			//OSFlagPost(Sem_Display, (OS_FLAGS)16, OS_FLAG_SET, &err);  Ҫ��Ҫ�ͷ��ź���
		}
		else if(Sem_Display->OSFlagFlags&0x20)
		{
			LED_DLY_ms(5000);
	    OLED_ShowString(48,0,"LiMin Lady:");	
		  OSFlagPost(Sem_Display, (OS_FLAGS)32, OS_FLAG_SET, &err);
		}
		else if(Sem_Display->OSFlagFlags&0x40)
		{
			OLED_DLY_ms(5000);
	    OLED_ShowString(8,0,"Anyway");	
		  //OSFlagPost(Sem_Display, (OS_FLAGS)64, OS_FLAG_SET, &err);
		}
		else
			OLED_DLY_ms(5000);
	    OLED_ShowString(8,0,"Hello");	
		// OSFlagPost(Sem_Display, (OS_FLAGS)128, OS_FLAG_SET, &err);	
  }
}

/**
  * @brief  Button update task
  *
  * @param  p_arg: Unused
  *
  * @retval None
  */
void ButtonUpdate(void *p_arg)
{
	INT8U error;
  while (1)
  {
		
		OSFlagPend(Sem_Display, (OS_FLAGS)15, OS_FLAG_WAIT_SET_ANY, 0, &err);  //����0~3λ
		// ������
		if(Sem_Display->OSFlagFlags&0x01)
		{
			
			OSFlagPost(Sem_Display, (OS_FLAGS)16, OS_FLAG_SET, &err);
		}
		else if(Sem_Display->OSFlagFlags&0x02)
		{
		  OSFlagPost(Sem_Display, (OS_FLAGS)32, OS_FLAG_SET, &err);
		}
		else if(Sem_Display->OSFlagFlags&0x04)
		{
		  OSFlagPost(Sem_Display, (OS_FLAGS)64, OS_FLAG_SET, &err);
		}
		else
		 OSFlagPost(Sem_Display, (OS_FLAGS)128, OS_FLAG_SET, &err);	
    OSTaskSuspend(OS_PRIO_SELF);
		
  }
}
