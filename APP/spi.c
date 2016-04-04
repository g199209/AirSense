/**
  **************************************************************
  * @file       spi.c
  * @author     ldliu
  * @version    V0.1
  * @date       2016-4-1
  *
  * @brief      HMI通信模块
  *
  * @details 
  * @verbatim
  * 
  *
  * @endverbatim
  ***************************************************************
  */

#include "spi.h"

#include <STM32F10X_SPI.h>
#include <STM32F10X_GPIO.h>
#include <STM32F10X_RCC.h>

/**
  * @brief  SPIx_Init function initialization
  *
  * @param  None
  *
  * @retval None
  */
void SPIx_Init(void)
{
	  SPI_InitTypeDef  SPI_InitStructure;
	  GPIO_InitTypeDef GPIO_InitStructure;

	  RCC_APB2PeriphClockCmd( RCC_APB2Periph_SPI1 | RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	
	  GPIO_InitStructure.GPIO_Pin = OLED_SCK_PIN | OLED_SDA_PIN ;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_Init(OLED_PORT, &GPIO_InitStructure);

	  /* GPIO4  */
	 //SPI1 RST 还是有点疑问
    GPIO_InitStructure.GPIO_Pin =OLED_RST_PIN | GPIO_Pin_8 ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	  // 设置DC引脚
	  GPIO_InitStructure.GPIO_Pin = OLED_DC_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

	  /* SPI1 Config -------------------------------------------------------------*/
	  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
	  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	  SPI_InitStructure.SPI_CRCPolynomial = 7;
	  SPI_Init(SPI1, &SPI_InitStructure);
	  /* Enable SPI1 */
	  SPI_Cmd(SPI1, ENABLE);
	
}   


/**
  * @brief  This function receive and send data.
  *
  * @param  uint8_t: the char that send out.
  *
  * @retval uint8_t: the char that receive.
  */
uint8_t  OLED_WB(uint8_t data)
{
  uint8_t retry=0;				 
	while((SPI1->SR&1<<1)==0)//等待发送区空	
	{
		retry++;
		if(retry>200)return 0;
	}			  
	SPI1->DR=data;	 	  //发送一个byte 
	retry=0;
	while((SPI1->SR&1<<0)==0) //等待接收完一个byte  
	{
		retry++;
		if(retry>200)return 0;
	}	  						    
	return SPI1->DR;          //返回收到的数据				    
}


/**
  * @brief  This function handle delaying time.
  *
  * @param  u16: the set delay time .
  *
  * @retval none
  */
void delay_us1(u16 time)
{
	u16 i = 0;
	while(time -- )
	{
		i = 10;
		while(i --);
	}
}
