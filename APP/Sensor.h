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

#define TaskDelayTime 15u    // Delay seconds

/**
  * @brief  Sensor measurement data type
  */
typedef struct
{
  uint32_t Humidity;
  int32_t Temp;
  uint32_t VOC;
  uint32_t PM25;
  // time_t Time;
} SensorDataType;

extern volatile SensorDataType SensorMeasureData;

extern __IO uint16_t ADC_PM25_ConvertedValue;
extern __IO uint16_t ADC_VOC_ConvertedValue;

ErrorStatus SensorInit(void);
void SensorMeasure(void *p_arg);
void Tim2_delay(u32 time_us);
#endif
