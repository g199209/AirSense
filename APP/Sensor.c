/****************************************************************
  * @file       Sensor.c
  * @author     高明飞
  * @version    V0.1
  * @date       2016-4-3
  *
  * @brief      传感器模块驱动
  *
  * @details 
  * @verbatim
  * 传感器模块HAL层程序源文件
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
  
#include "BSP.h"

/*The macro is used to control the led of VOC sampling*/
#define VOC_ON   	GPIO_SetBits(GPIOB,GPIO_Pin_10)
#define VOC_OFF   GPIO_ResetBits(GPIOB,GPIO_Pin_10)

/*The macro is used to control the led of VOC sampling*/
#define PM25_ON   GPIO_SetBits(GPIOB,GPIO_Pin_1)
#define PM25_OFF  GPIO_ResetBits(GPIOB,GPIO_Pin_1)

/*The MACRO is used to control Tim2*/
#define TIM2_ON   TIM_Cmd(TIM2, ENABLE)
#define TIM2_OFF  TIM_Cmd(TIM2, DISABLE)

/*The data register adress of ADC1*/
#define ADC1_DR_Address    ((u32)0x40012400+0x4c)

/* ADC_RESULTS[0] is used to store the value of PM2.5 */
/* ADC_RESULTS[1] is used to store the value of VOC */
volatile uint16_t ADC_RESULTS[2];

/*A varialbe is used to COUNT in TIM2*/
__IO uint32_t TIM2_count;

volatile SensorDataType SensorMeasureData;

/*********************************************PM2.5  AREA*******************************************/

/**
  * @brief  Module PM2.5 GPIO of CON LED(PB1) and AIN1(PA0) Configuration
  *
  * @param  None
  *
  * @retval ERROR: Initialization failed
  * @retval SUCCESS: Initialization successed
  */
ErrorStatus GPIO_PM25_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA, ENABLE);
	
	/*PB1 is used to lighten the CON LED*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/*PAO is input channel 0 of ADC12*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	return SUCCESS;
	
}

/**
  * @brief  ADC input channel Configuration, including PM2.5 & VOC
  *
  * @param  None
  *
  * @retval ERROR: Initialization failed
  * @retval SUCCESS: Initialization successed
  */
ErrorStatus ADC1_Init(void)
{
		DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
		
	/* Enable DMA clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	/* Enable ADC1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	/* DMA channel1 configuration */
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;	 //ADC data register address 
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_RESULTS;//memory address
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 2;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//fixed peripehral address
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //Fixed memory address
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//halfword
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;		//cycle transmit
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	/* Enable DMA channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	/* ADC1 configuration */
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//independent ADC mode 
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 	 //channel scan disable
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 
	ADC_InitStructure.ADC_NbrOfChannel = 2;	 	//only one channel
	ADC_Init(ADC1, &ADC_InitStructure);
	
	/*The maxmium CLK of ADC is 14MHZ*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 

  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_1Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_1Cycles5);
	
	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);
	
	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
	 
	ADC_ResetCalibration(ADC1);
	
	while(ADC_GetResetCalibrationStatus(ADC1));

	ADC_StartCalibration(ADC1);

	while(ADC_GetCalibrationStatus(ADC1));
	 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	
	return SUCCESS;
}

/**
  * @brief  Module PM2.5 Initialization
  *
  * @param  None
  *
  * @retval ERROR: Initialization failed
  * @retval SUCCESS: Initialization successed
  */
ErrorStatus PM25_Init()
{
	GPIO_PM25_Config();
	return SUCCESS;
}

/**
  * @brief  Module PM2.5 sampling
  *
  * @param  None
  *
  * @retval None
  */
void PM25_Sample(uint32_t *pPM25)
{
	__IO uint32_t temp1;
	
	/*open peripheral TIM2*/
	//TIM2_ON;
	
			PM25_ON;
			
			Tim2_delay(50);
			
			temp1 = ADC_RESULTS[0];
		
		  Tim2_delay(14);
		
			PM25_OFF;
		
			*pPM25 =(uint32_t) temp1 * 0.96387; 

	/*close peripheral TIM2*/		
  //TIM2_OFF;		
}

/*********************************************VOC  AREA*******************************************/
/**
  * @brief  Module VOC GPIO of CON LED(PB10) and AIN1(PA1) Configuration
  *
  * @param  None
  *
  * @retval ERROR: Initialization failed
  * @retval SUCCESS: Initialization successed
  */
ErrorStatus GPIO_VOC_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA, ENABLE);
	
	/*PB10 is used to lighten the CON VN*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_SetBits(GPIOB, GPIO_Pin_10);
	
	/*PAO is input channel 1 of ADC12*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	return SUCCESS;
	
}

/**
  * @brief  Module VOC Initialization
  *
  * @param  None
  *
  * @retval ERROR: Initialization failed
  * @retval SUCCESS: Initialization successed
  */
ErrorStatus VOC_Init()
{
	GPIO_VOC_Config();
	
	return SUCCESS;
}

/**
  * @brief  Module VOC sampling
  *
  * @param  None
  *
  * @retval None
  */
void VOC_Sample(uint32_t *pVOC)
{
	//__IO uint16_t temp1;
	
	/*open peripheral TIM2*/
	// TIM2_ON;
	
			// VOC_ON;
			
			//Tim2_delay(64);
			
			//temp1 = ADC_VOC_ConvertedValue;
		
			// VOC_OFF;
		
			// *pVOC =(float) temp1/4096*3.3; 
      // Unit : Ohm
  *pVOC = VOCMAX - (63286364 / (uint32_t)ADC_RESULTS[1] - 10200);
	
	/*close peripheral TIM2*/		
  // TIM2_OFF;		
	
}

/*********************************************DHT11  AREA*******************************************/
ErrorStatus GPIO_DHT11_Config(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;
	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);

	/*Remap the PB3 to disable the function of SWJ_JTAG*/
	 GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	 GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	 return SUCCESS;
}

/**
  * @brief  Module Tempeture and humity Initialization
  *
  * @param  None
  *
  * @retval ERROR: Initialization failed
  * @retval SUCCESS: Initialization successed
  */
ErrorStatus DHT11_Init()
{
	GPIO_DHT11_Config();
	return SUCCESS;
}

/**
  * @brief  Detect tempeture and humity 
  * @param  None
  * @retval ERROR: Initialization failed
  * @retval SUCCESS: Initialization successed
  */
ErrorStatus Temperature_Sample(uint32_t *pTemperature, uint32_t *pHumity)
{
	u32 count;
	u32 i;
	
	u8 temp;
	u32 input = 0;
	
	/*Integral part of temperature*/
	u32 Tem_int = 0;
	/*Decimal part of temperature*/
  u32 Tem_dec = 0;
	/*Integral part of humity*/
	u32 Hum_int = 0; 
	/*Decimal part of humity*/
  u32 Hum_dec = 0;
	
	/*check byte*/
	u8 ReceiveCheck = 0;
	/*4 byte data*/
	u32 ReceiData = 0;
	
/********************************send start signal********************/
  GPIOB->CRL &= 0xFFFF0FFF;
  GPIOB->CRL |= 0x00007000;
  
  GPIOB->BRR = GPIO_Pin_3;    // Reset
	
	Tim2_delay(4000);
	
  GPIOB->BSRR = GPIO_Pin_3;   // Set
/*******************************start signal end**********************/

/************config PB3 as input mode*********************************/
	GPIOB->CRL &= 0XFFFF0FFF;
	GPIOB->CRL |= 8<<12;										

/****************wait DHT11 respond**********************************/
	while((GPIOB ->IDR & GPIO_Pin_3) != Bit_RESET);
	 
  while ((GPIOB->IDR & GPIO_Pin_3) == Bit_RESET);

  while ((GPIOB->IDR & GPIO_Pin_3) == GPIO_Pin_3);

/****************read DHT11 data*************************************/
	input = GPIOB ->IDR & GPIO_Pin_3;
	
	/*four byte data consist of two byte temperature and two byte humity data*/
	for(i=0x80000000, count = 0; i>0 ; i>>=1)
	{
	  while(input == Bit_RESET)
		{
			input = GPIOB ->IDR & GPIO_Pin_3;
		}		
		
		 while(input == GPIO_Pin_3)
		{
			input = GPIOB ->IDR & GPIO_Pin_3;
			count += 1;
		}
	
		if(count >= 70)
		{
			ReceiData |= i;	
		}
		count = 0;
	}
	
	/*one byte check data*/
	for(i=0x80; i>0 ; i>>=1)
	{
	  while(input == Bit_RESET)
		{
			input = GPIOB ->IDR & GPIO_Pin_3;
		}

		 while(input == 8)
		{
			input = GPIOB ->IDR & GPIO_Pin_3;
			count += 1;
		}
		
		if(count >= 76)
		{
			ReceiveCheck |= i;	
		}
    count = 0;
	}
	
	/*        byte4   byte3   byte2   byte1   byte0                                */
	/*        整数____小数    整数____小数    校验和                               */
  /*            湿度            温度        校验和                               */
	Tem_int = (ReceiData & 0x0000FF00) >> 8;
	Tem_dec = (ReceiData & 0x000000FF);
	
	Hum_int = (ReceiData & 0xFF000000) >> 24;
	Hum_dec = (ReceiData & 0X00FF0000) >> 16;
	
	temp = Tem_int + Tem_dec + Hum_int + Hum_dec;

  *pTemperature = Tem_int;
  *pHumity = Hum_int;

  return SUCCESS;
	
	/*    humi(湿度)  = byte4 . byte3  (%RH)                                       */
	/*    temp(温度)  = byte2 . byte1  ('C)                                        */
	/*    check(校验) = byte4 + byte3 + byte2 + byte1 = humi + temp(校验正确)      */
	
	//if(temp == ReceiveCheck)
	//{
	//	*pTemperature = Tem_int + 0.1*Tem_dec;
	//	*pHumity = Hum_int + 0.1*Hum_dec;		
	//	return SUCCESS;
	//}	
	//else
	//	return ERROR;
	
}
/*********************************************Timer2  AREA*******************************************/
/**
  * @brief  Moudle TIM2 configuration
  * @param  None
  * @retval None
  */
void 	TIM2_Configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
  
	/*Enable the CLK of TIM2*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

	
	TIM_TimeBaseInitStructure.TIM_Period = 359;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 0;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
	/*Initilize the TIM2*/
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
  
	/*Clear the flag bit of overflow interrupt*/
	TIM_ClearFlag(TIM2,TIM_IT_Update);

	/*Enable the overflow interrupt*/
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	/*Enable peripheral TIMW2*/
	TIM_Cmd(TIM2,ENABLE);
}

/**
  * @brief  Nesting vector interrrupt control Initilization
  * @param  None
  * @retval None
  */
void TIM2_NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStruceture;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	NVIC_InitStruceture.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruceture.NVIC_IRQChannelPreemptionPriority = TIM2IRQPRIO;
	NVIC_InitStruceture.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruceture.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStruceture);
}

/**
  * @brief  Tim2 interrupt service handler
  * @param  None
  * @retval None
  */
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)	
	{
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
		TIM2_count++;
		return;
	}		
}

/**
  * @brief  The function is used to delay time_us
  * @param  None
  * @retval None
  */
void Tim2_delay(u32 time_us)
{
	
	// TIM2_ON;
	
	TIM2_count = 0;	
	while(time_us >= TIM2_count);
  TIM2_count = 0;
	
	// TIM2_OFF;
	
}
/*********************************************All Sensor initalization AREA*******************************************/
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
  uint32_t tmp;
	PM25_Init();
	VOC_Init();
  ADC1_Init();
	DHT11_Init();
	TIM2_Configuration();
	TIM2_NVIC_Configuration();

  Temperature_Sample(&tmp, &tmp);
  PM25_Sample(&tmp);
  VOC_Sample(&tmp);

  return SUCCESS;
}

/*********************************************All sensor sampling AREA*******************************************/
/**
  * @brief  Sensor measurement task
  *
  * @param  p_arg: Unused
  *
  * @retval None
  */
void SensorMeasure(void *p_arg)
{
  INT8U err;
  uint32_t ptem, phum, pPM, pVOC;
	
  while (1)
  {
#ifdef __DEBUG
    printf("Start Sensor Measurement taks!\r\n");
    printf("Start Temperature Sampling!\r\n");
#endif
    Temperature_Sample(&SensorMeasureData.Temp, &SensorMeasureData.Humidity);
#ifdef __DEBUG
    printf("Start PM2.5 Sampling!\r\n");
#endif
    PM25_Sample(&SensorMeasureData.PM25);
#ifdef __DEBUG
    printf("Start VOC Sampling!\r\n");
#endif
    VOC_Sample(&SensorMeasureData.VOC);
		
    //SensorMeasureData.Time = time(0);

#ifdef __DEBUG
    printf("\r\nMeasurement results:\r\nTmeparure : %d\r\nHumidity : %d\r\nPM2.5 : %d\r\nVOC : %d\r\n\r\n",\
      SensorMeasureData.Temp, SensorMeasureData.Humidity, SensorMeasureData.PM25, SensorMeasureData.VOC);
#endif

    if (TCP_Connected)
    {
#ifdef __DEBUG
      printf("\r\nPost SemSensorDataReady!\r\n");
#endif
      err = OSSemPost(SemSensorDataReady);
    }
    OSFlagPost(Sem_Display, FLAGS_MEASURE, OS_FLAG_SET, &err);
    OSTimeDlyHMSM(0, 0, TaskDelayTime, 0);
  }
}
