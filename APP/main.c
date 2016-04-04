/**
  **************************************************************
  * @file       main.c
  * @author     高明飞
  * @version    V0.1
  * @date       2016-4-3
  *
  * @brief      主程序
  *
  * @details 
  * @verbatim
  * 程序入口，调用系统初始化函数及OS初始化；
  *
  * 修改记录：
  * 2016-4-1 :
  *   - File Created.
  *
  * @endverbatim
  ***************************************************************
  */

/* Include header files */
#include "BSP.h"

extern OS_STK Task_Init_STK[TASK_INIT_STK_SIZE];

/**
  * @brief  C语言程序入口
  *
  * @param  None
  *
  * @retval 0
  */
int main(void)
{
  BSPInit();

#ifdef __DEBUG
  printf("BSP initialization OK!\r\n");
#endif

  OSInit();
  
  OSTaskCreate(TaskInit, (void *)0, &Task_Init_STK[TASK_INIT_STK_SIZE - 1], TASK_INIT_PRIO);

#ifdef __DEBUG
  printf("\r\n");
  printf("*************************\r\n");
  printf("#### uC/OS II Start! ####\r\n");
  printf("*************************\r\n");
  printf("\r\n");
#endif

  OSStart();

  for (;;);
}


