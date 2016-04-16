/**
  **************************************************************
  * @file       RTC.h
  * @author     ������
  * @version    V0.1
  * @date       2016-4-4
  *
  * @brief      RTC module
  *
  * @details 
  * @verbatim
  * RTCģ�鼰ʱ�䴦��
  *
  * �޸ļ�¼��
  * 2016-4-4 :
  *   - File Created.
  *
  * @endverbatim
  ***************************************************************
  */

#ifndef RTC_H
#define RTC_H

void RTCInit(void);

//���ݼ�⵽������������ݣ�����⵽������ת��Ϊ��Ӧʱ�䵥λ����
uint32_t Time_Regulate(void);

//���Time_Regulate���ص���ֵ�趨RTC�ļ����Ĵ���CNTH��CNTL
void Time_Adjust(void);

//ͨ�����ڼ������Ҫ�趨��ʱ��
uint8_t USART_Scanf(uint32_t value);

#endif
