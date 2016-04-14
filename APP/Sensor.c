/**
  **************************************************************
  * @file       Sensor.c
  * @author     高明飞
  * @version    V0.1
  * @date       2016-4-3
  *
  * @brief      传感器模块驱动
  *
  * @details 
  * @verbatim
  * 传感器模块HAL层程序源文件
  *
  * 包括PM2.5传感器、温湿度传感器、VOC传感器
  *
  * 修改记录：
  * 2016-4-3 :
  *   - File Created.
  *
  * @endverbatim
  ***************************************************************
  */
  
#include "BSP.h"

volatile SensorDataType SensorMeasureData;

/**
  * @brief  Sensor initialization
  *
  * @param  None
  *
  * @retval ERROR: Initialization failed
  * @retval SUCCESS: Initialization successed
  */
ErrorStatus SensorInit(void)
{
  return SUCCESS;
}

/**
  * @brief  Sensor measurement task
  *
  * @param  p_arg: Unused
  *
  * @retval None
  */
void SensorMeasure(void *p_arg)
{
  INT8U err;
  
  while (1)
  {
    /* Test */
    SensorMeasureData.PM25 = 546;
    SensorMeasureData.Temp = 19120;
    SensorMeasureData.Humidity = 60340;
    SensorMeasureData.VOC = 320;
    //SensorMeasureData.Time = time(0);

    if (TCP_Connected)
    {
#ifdef __DEBUG
      printf("\r\nPost SemSensorDataReady!\r\n");
#endif
      err = OSSemPost(SemSensorDataReady);
    }
    OSFlagPost(Sem_Display, FLAGS_MEASURE, OS_FLAG_SET, &err);
    OSTimeDlyHMSM(0, 0, TaskDelayTime, 0);
  }
}
