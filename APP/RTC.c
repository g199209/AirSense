/**
  **************************************************************
  * @file       RTC.c
  * @author     ������
  * @version    V0.1
  * @date       2016-4-4
  *
  * @brief      RTC module
  *
  * @details 
  * @verbatim
  * RTCģ�鼰ʱ�䴦��
  * RTC Counter��ʹ��UNIXʱ���������1970��1��1�����������
  *
  * �޸ļ�¼��
  * 2016-4-4 :
  *   - File Created.
  *
  * @endverbatim
  ***************************************************************
  */

#include "BSP.h"

/**
  * @brief  RTC initialization
  *
  * @param  None
  *
  * @retval None
  */
void RTCInit(void)
{

}

/**
  * @brief  ʵ�ֱ�׼���е�time()�������˺���Ӧ��Ҫ����RTC Counter�е�ֵ
  *
  * @param  timer: Unused
  *
  * @retval time_t: Time
  */
time_t time(time_t * timer)
{
  return 7;
}


/**
* @brief  This function handles RTC seconds interrupt Handler
*
* @param  None
*
* @retval None
*/
void RTC_IRQHandler(void)
{
  OSIntEnter();
  if (RTC->CRL & RTC_CRL_SECF)
  {
    /* Clear the RTC Second interrupt */
    RTC->CRL &= ~RTC_CRL_SECF;

    /* Loop until RTOFF flag is set */
    while ((RTC->CRL & RTC_FLAG_RTOFF) == (uint16_t)RESET);
  }
  OSIntExit();
}
