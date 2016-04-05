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

 sem1=OSSemCreate(1);
 sem2=OSSemCreate(1);
 sem3=OSSemCreate(1);
 sem4=OSSemCreate(1);
 sem5=OSSemCreate(1);
 sem6=OSSemCreate(1);
 sem7=OSSemCreate(1);
 sem8=OSSemCreate(1);
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
		OSSemPend(sem5,0,&error);
		//������
		OSSemPend(sem6,0,&error);
		
		OSSemPend(sem7,0,&error);
		
		OSSemPend(sem8,0,&error);
		
    OSTaskSuspend(OS_PRIO_SELF);
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
		//�����if�������жϣ�
		OSSemPend(sem1,0,&error);
		// ������
		OSSemPost(sem5);
		
		OSSemPend(sem2,0,&error);
		
		OSSemPost(sem6);
		
		OSSemPend(sem3,0,&error);
		
		OSSemPost(sem7);
		
		OSSemPend(sem4,0,&error);
		
		OSSemPost(sem8);
    OSTaskSuspend(OS_PRIO_SELF);
		//OSSemPend();
  }
}
