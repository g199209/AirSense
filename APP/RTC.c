/**
  **************************************************************
  * @file       RTC.c
  * @author     高明飞
  * @version    V0.1
  * @date       2016-4-4
  *
  * @brief      RTC module
  *
  * @details 
  * @verbatim
  * RTC模块及时间处理
  * RTC Counter中使用UNIX时间戳，即自1970年1月1日至今的秒数
  *
  * 修改记录：
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
  * @brief  实现标准库中的time()函数，此函数应该要返回RTC Counter中的值
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
