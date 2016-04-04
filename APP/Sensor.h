/**
  **************************************************************
  * @file       Sensor.h
  * @author     ������
  * @version    V0.1
  * @date       2016-4-3
  *
  * @brief      ������ģ������
  *
  * @details 
  * @verbatim
  * ������ģ��HAL�����ͷ�ļ�
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

#ifndef SENSOR_H
#define SENSOR_H

#define TaskDelayTime 60u    // Delay seconds

/**
  * @brief  Sensor measurement data type
  */
typedef struct
{
  uint32_t PM25;
  uint32_t VOC;
  float Temp;
  float Humidity;
  time_t Time;
} SensorDataType;

ErrorStatus SensorInit(void);
void SensorMeasure(void *p_arg);

#endif
