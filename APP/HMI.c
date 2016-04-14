/**
  **************************************************************
  * @file       HMI.c
  * @author     高明飞
  * @version    V0.1
  * @date       2016-4-3
  *
  * @brief      HMI模块驱动
  *
  * @details 
  * @verbatim
  * HMI模块HAL层程序源文件
  *
  * 包括OLED屏幕及触摸按键
  *
  * 修改记录：
  * 2016-4-3 :
  *   - File Created.
  *
  * @endverbatim
  ***************************************************************
  */

#include "BSP.h"

//INT8U err;

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
  EXTIX_Init();
  SPIx_Init();
  OLED_Init();    //OLED初始化
	
  // Clear Screen
  OLED_CLS();  
	
  // OLED_DLY_ms(5000);

  /* Initial Display */
  OLED_ShowString(0, 00, "----AirSense----");
  OLED_ShowString(0, 16, "  Mingfei Gao   ");
  OLED_ShowString(0, 32, "   Xudong Ren   ");
  OLED_ShowString(0, 48, "   Dalong Liu   ");
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
  INT8U err;
  OS_FLAGS SemFlag;

  while (1)
  {
		//if语句进行判断
    SemFlag = OSFlagPend(Sem_Display, (OS_FLAGS)(0x1 + 0x2 + 0x4 + 0x8), OS_FLAG_WAIT_SET_ANY | OS_FLAG_CONSUME, 0, &err);  //请求0~3位
		
    OLED_Fill(0, 0, X_WIDTH - 1, Y_WIDTH - 1, 0x00);
    if (SemFlag & 0x1)
		{
	    OLED_ShowString(0, 0,"0x10");	
		}
    else if (SemFlag & 0x2)
		{
	    OLED_ShowString(0, 16,"0x20");	
		}
    else if (SemFlag & 0x4)
		{
	    OLED_ShowString(0, 32,"0x40");	
		}
    else if (SemFlag & 0x8)
    {
      OLED_ShowString(0, 48, "0x80");
    }
    OLED_Refresh_Gram();
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
	INT8U err;
  while (1)
  {
		
		OSFlagPend(Sem_Display, (OS_FLAGS)15, OS_FLAG_WAIT_SET_ANY, 0, &err);  //请求0~3位
		// 代码区
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


