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
  while (1)
  {
    /* Test */
    SensorMeasureData.PM25 = 34;
    SensorMeasureData.Temp = 23120;
    SensorMeasureData.Humidity = 45340;
    SensorMeasureData.VOC = 67;
    //SensorMeasureData.Time = time(0);

//#ifdef __DEBUG
//    printf("Post SemSensorDataReady!\r\n");
//#endif
    OSSemPost(SemSensorDataReady);
    OSTimeDlyHMSM(0, 0, TaskDelayTime, 0);
  }
}
