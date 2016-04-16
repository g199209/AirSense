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
	/***************************************进行中断控制器配置**************************************************/

	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Configure one bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	/* Enable the RTC Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/***************************************RTC的初始化*********************************************************/
	/* Enable PWR and BKP clocks */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	
	/* Allow access to BKP Domain */
	PWR_BackupAccessCmd(ENABLE);
	
	/* Reset Backup Domain */
	BKP_DeInit();
	
	/* Enable LSE */
	RCC_LSEConfig(RCC_LSE_ON);
	/* Wait till LSE is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
	{}
	
	/* Select LSE as RTC Clock Source */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	
	/* Enable RTC Clock */
	RCC_RTCCLKCmd(ENABLE);
	
	/* Wait for RTC registers synchronization */
	RTC_WaitForSynchro();
	
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
	
	/* Enable the RTC Second */
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
	
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
	
	/* Set RTC prescaler: set RTC period to 1sec */
	RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
	
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();

}

/**
* @brief  检测从串口中获得的数据
*
* @param  None
*
* @retval None
*/

uint8_t USART_Scanf(uint32_t value)
{
	uint32_t index = 0;
	uint32_t tmp[2] = {0, 0};
	
	while (index < 2)
	{
		/* Loop until RXNE = 1 */
		while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET)
		{}
		tmp[index++] = (USART_ReceiveData(USART2));
	
		if ((tmp[index - 1] < 0x30) || (tmp[index - 1] > 0x39))
		{
			printf("\n\rPlease enter valid number between 0 and 9");
			index--;
			tmp[index] = 0;
		}
	}
	/* Calculate the Corresponding value */
	index = (tmp[1] - 0x30) + ((tmp[0] - 0x30) * 10);
	/* Checks */
	if (index > value)
	{
		printf("\n\rPlease enter valid number between 0 and %d", value);
		return 0xFF;
	}
	return index;
}


/**
* @brief  返回在串口中输入的时间值
*
* @param  None
*
* @retval None
*/

uint32_t Time_Regulate(void)
{
	uint32_t Tmp_HH = 0xFF, Tmp_MM = 0xFF, Tmp_SS = 0xFF;
	
	printf("\r\n==============Time Settings=====================================");
	printf("\r\n  Please Set Hours");
	
	while (Tmp_HH == 0xFF)
	{
		Tmp_HH = USART_Scanf(23);
	}
	printf(":  %d", Tmp_HH);
	printf("\r\n  Please Set Minutes");
	while (Tmp_MM == 0xFF)
	{
		Tmp_MM = USART_Scanf(59);
	}
	printf(":  %d", Tmp_MM);
	printf("\r\n  Please Set Seconds");
	while (Tmp_SS == 0xFF)
	{
		Tmp_SS = USART_Scanf(59);
	}
	printf(":  %d", Tmp_SS);
	
	/* Return the value to store in RTC counter register */
	return((Tmp_HH*3600 + Tmp_MM*60 + Tmp_SS));
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
* @brief  时间调节
*
* @param  None
*
* @retval None
*/

void Time_Adjust(void)
{
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
	/* Change the current time */
	RTC_SetCounter(Time_Regulate());
	/* Wait until last write operation on RTC registers has finished */
	RTC_WaitForLastTask();
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
