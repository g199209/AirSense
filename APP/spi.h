/**
  **************************************************************
  * @file       spi.h
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
	
#ifndef __SPI_H
#define __SPI_H


/* defines */
#define     OLED_PORT                       GPIOA
#define     OLED_RST_PIN                    GPIO_Pin_4

#define     OLED_RST_L                      (OLED_PORT->BRR = OLED_RST_PIN)
#define     OLED_RST_H                      (OLED_PORT->BSRR = OLED_RST_PIN)

#define     OLED_DC_PORT                    GPIOB
#define     OLED_DC_PIN                     GPIO_Pin_0

#define	    OLED_DC_L                       (OLED_DC_PORT->BRR = OLED_DC_PIN)
#define     OLED_DC_H                       (OLED_DC_PORT->BSRR = OLED_DC_PIN)

#define  	  OLED_SCK_PIN					          GPIO_Pin_5  
#define		  OLED_SDA_PIN					          GPIO_Pin_7

// SPI总线速度设置 
#define SPI_SPEED_2   0
#define SPI_SPEED_8   1
#define SPI_SPEED_16  2
#define SPI_SPEED_256 3
						  	    													  
void SPIx_Init(void);			      //初始化SPI口
uint8_t  OLED_WB(uint8_t data);
void delay_us1(u16 time);

#endif
