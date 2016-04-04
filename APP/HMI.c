/**
  **************************************************************
  * @file       HMI.c
  * @author     ������
  * @version    V0.1
  * @date       2016-4-3
  *
  * @brief      HMIģ������
  *
  * @details 
  * @verbatim
  * HMIģ��HAL�����Դ�ļ�
  *
  * ����OLED��Ļ����������
  *
  * �޸ļ�¼��
  * 2016-4-3 :
  *   - File Created.
  *
  * @endverbatim
  ***************************************************************
  */

#include "BSP.h"

/**
  * @brief  HMI module initialization
  *
  * @param  None
  *
  * @retval ERROR: Initialization failed
  * @retval SUCCESS: Initialization successed
  */
ErrorStatus HMIInit(void)
{
  return SUCCESS;
}

/**
  * @brief  OLED display update task
  *
  * @param  p_arg: Unused
  *
  * @retval None
  */
void OLEDUpdate(void *p_arg)
{

  while (1);
}

/**
  * @brief  Button update task
  *
  * @param  p_arg: Unused
  *
  * @retval None
  */
void ButtonUpdate(void *p_arg)
{

  while (1);
}
