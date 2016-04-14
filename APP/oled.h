/**
  **************************************************************
  * @file       oled.h
  * @author     ldliu
  * @version    V0.1
  * @date       2016-4-1
  *
  * @brief      HMI显示模块
  *
  * @details 
  * @verbatim
  * 
  *
  * @endverbatim
  ***************************************************************
  */

#ifndef __OLED_H
#define __OLED_H

/* includes */
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "spi.h" 

#define	    BRIGHTNESS	  0xCF			//亮度
#define     X_WIDTH       128				//宽度
#define     Y_WIDTH       64				//长度
#define		  PAGE			    8					//页数
#define			MAX_CHAR_POSX X_WIDTH-6 //字符宽度需要-6
#define			MAX_CHAR_POSY Y_WIDTH-6 //字符长度需要-6

   
/* OLED_WriteByte */
 void OLED_DLY_ms(unsigned int ms);
 void delay_us(u16 time);
 void LCD_Init(void);
 void OLED_CLS(void);
 void LCD_Fill(unsigned char dat);
 void OLED_Init(void);

//SSD1306 OLED 驱动IC驱动代码
//驱动方式:SPI
//版本:V0.1

//2016-3-31

//OLED模式设置
//4线串行模式
		    						  
//OLED控制用函数
 void OLED_Display_On(void);
 void OLED_Display_Off(void);
 void OLED_Refresh_Gram(void);		   
							   		    
 void OLED_DrawPoint(u8 x,u8 y,u8 t);
 void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
 void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
 void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
 void OLED_ShowString(u8 x,u8 y,const u8 *p);	

#endif
