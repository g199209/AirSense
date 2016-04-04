/**
  **************************************************************
  * @file       Sensor.c
  * @author     ������
  * @version    V0.1
  * @date       2016-4-3
  *
  * @brief      ������ģ������
  *
  * @details 
  * @verbatim
  * ������ģ��HAL�����Դ�ļ�
  *
  * ����PM2.5����������ʪ�ȴ�������VOC������
  *
  * �޸ļ�¼��
  * 2016-4-3 :
  *   - File Created.
  *
  * @endverbatim
  ***************************************************************
  */
  
#include "BSP.h"

SensorDataType SensorMeasureData;

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
  while (1)
  {
    /* Test */
    SensorMeasureData.PM25 = 200;
    SensorMeasureData.Temp = 25.4;
    SensorMeasureData.Humidity = 0.65;
    SensorMeasureData.VOC = 100;
    SensorMeasureData.Time = time(0);

    OSTimeDlyHMSM(0, 0, TaskDelayTime, 0);
  }
}
