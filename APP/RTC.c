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
    /* Reset RTC Counter when Time is 23:59:59 */
    if (RTC->CNTH == 0x0001 && RTC->CNTL == 0x5180)
    {
      RTC->CNTH = 0x00;
      RTC->CNTL = 0x00;
      /* Loop until RTOFF flag is set */
      while ((RTC->CRL & RTC_FLAG_RTOFF) == (uint16_t)RESET);
    }

    /* Clear the RTC Second interrupt */
    RTC->CRL &= ~RTC_CRL_SECF;

    /* Loop until RTOFF flag is set */
    while ((RTC->CRL & RTC_FLAG_RTOFF) == (uint16_t)RESET);
  }
  OSIntExit();
}
