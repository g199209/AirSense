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
void TimeCNTToHMS(uint32_t TimeCNT, uint32_t * Hours, uint32_t * Minutes, uint32_t * Seconds);
time_t EncodeTimestamp(uint32_t Year, uint32_t Mounth, uint32_t Day, uint32_t TimeCNT);
#endif
