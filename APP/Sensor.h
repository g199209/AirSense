/**
  **************************************************************
  * @file       Sensor.h
  * @author     高明飞
  * @version    V0.1
  * @date       2016-4-3
  *
  * @brief      传感器模块驱动
  *
  * @details 
  * @verbatim
  * 传感器模块HAL层程序头文件
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

ErrorStatus SensorInit(void);
void SensorMeasure(void *p_arg);

#endif
