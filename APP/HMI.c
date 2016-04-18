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

DisplayContent CurrentDisplayContent;

static void StartScreen(void);
static void MeasurementDisplay(void);

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
	
  StartScreen();
	
  return SUCCESS;
}

/**
  * @brief  Start Screen
  *
  * @param  None
  *
  * @retval None
  */
static void StartScreen(void)
{
  // Clear Screen
  OLED_Fill(0, 0, X_WIDTH - 1, Y_WIDTH - 1, 0x00);

  OLED_ShowString(0, 00, "----AirSense----");
  OLED_ShowString(0, 16, "  Mingfei Gao   ");
  OLED_ShowString(0, 32, "   Xudong Ren   ");
  OLED_ShowString(0, 48, "   Dalong Liu   ");

  OLED_Refresh_Gram();
}

static void MeasurementDisplay(void)
{
  // Do not need to clear screen, which is done in OLEDUpdate()

  OLED_ShowString(0, 00, "Tempature:     ");
  OLED_ShowNum(88, 0, SensorMeasureData.Temp, 2, 16);

  OLED_ShowString(0, 16, "Humidity :    %");
  OLED_ShowNum(88, 16, SensorMeasureData.Humidity, 2, 16);

  OLED_ShowString(0, 32, "PM2.5 :");
  OLED_ShowNum(64, 32, SensorMeasureData.PM25, 5, 16);

  OLED_ShowString(0, 48, "VOC   :");
  OLED_ShowNum(64, 48, SensorMeasureData.VOC, 5, 16);
  
  // Do not need to call OLED_Refresh_Gram(), which is done in OLEDUpdate()
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
    SemFlag = OSFlagPend(Sem_Display, FLAG_GRP, OS_FLAG_WAIT_SET_ANY | OS_FLAG_CONSUME, 0, &err);  //请求0~3位
		
    // Clear Screen
    OLED_Fill(0, 0, X_WIDTH - 1, Y_WIDTH - 1, 0x00);

    if (SemFlag & FLAGS_BTN1)
		{
      MeasurementDisplay();
		}
    else if (SemFlag & FLAGS_BTN2)
		{
	    OLED_ShowString(0, 16,"0x20");
      
		}
    else if (SemFlag & FLAGS_BTN3)
		{
	    // OLED_ShowString(0, 32,"0x40");
      // Airkiss
		}
    else if (SemFlag & FLAGS_BTN4)
    {
      OLED_ShowString(0, 48, "0x80");
    }
    else if (SemFlag & FLAGS_MEASURE)
    {
      MeasurementDisplay();
    }
    else if (SemFlag & FLAGS_AirStart)
    {
      OLED_ShowString(20, 24, "Airkiss....");
    }
    else if (SemFlag & FLAGS_AirOK)
    {
      OLED_ShowString(20, 24, "Airkiss OK");
    }
    else if (SemFlag & FLAGS_AirERROR)
    {
      OLED_ShowString(20, 24, "Airkiss ERROR");
    }
    OLED_Refresh_Gram();
  }
}


