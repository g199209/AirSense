/**
  **************************************************************
  * @file       exti.h
  * @author     ldliu
  * @version    V0.1
  * @date       2016-4-1
  *
  * @brief      °´¼üÖÐ¶ÏÄ£¿é
  *
  * @details 
  * @verbatim
  * 
  *
  * @endverbatim
  ***************************************************************
  */

#ifndef __EXTI_H
#define __EXIT_H	

/* Define interrupt priority */
#define EXTIIRQPRIO 9

void NVIC_GroupConfig(void);  	 
void EXTIX_Init(void);	

#endif
