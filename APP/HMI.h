/**
  **************************************************************
  * @file       HMI.h
  * @author     ������
  * @version    V0.1
  * @date       2016-4-3
  *
  * @brief      HMIģ������
  *
  * @details 
  * @verbatim
  * HMIģ��HAL�����ͷ�ļ�
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

#ifndef HMI_H
#define HMI_H

#include "spi.h"
#include "oled.h"
#include "exti.h"
#include "ucos_ii.h"
 


ErrorStatus HMIInit(void);

void OLEDUpdate(void *p_arg);
void ButtonUpdate(void *p_arg);

#endif
