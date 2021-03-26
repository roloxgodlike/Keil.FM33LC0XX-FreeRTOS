/**
  ****************************************************************************************************
  * @file    fm33lc0xx_fl_iwdt.c
  * @author  FMSH Application Team
  * @brief   Src file of IWDT FL Module
  ****************************************************************************************************
  * @attention
  *
  * Copyright (c) [2019] [Fudan Microelectronics]
  * THIS SOFTWARE is licensed under the Mulan PSL v1.
  * can use this software according to the terms and conditions of the Mulan PSL v1.
  * You may obtain a copy of Mulan PSL v1 at:
  * http://license.coscl.org.cn/MulanPSL
  * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR
  * IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR
  * PURPOSE.
  * See the Mulan PSL v1 for more details.
  *
  ****************************************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "fm33lc0xx_fl_rcc.h"
#include "fm33lc0xx_fl_iwdt.h"
#include "fm33_assert.h"
/** @addtogroup FM33LC0xx_FL_Driver_IWDT
  * @{
  */

/** @addtogroup Private_Macros
  * @{
  */
#define         IS_IWDT_INSTANCE(INTANCE)                    ((INTANCE) == IWDT)

#define         IS_FL_IWDT_WINDOWSVEL(__VALUE__)                ((__VALUE__) < 0xFFF)

#define         IS_FL_IWDT_OVERFLOWPERIOD(__VALUE__)            (((__VALUE__) == FL_IWDT_PERIOD_125MS)||\
                                                                 ((__VALUE__) == FL_IWDT_PERIOD_250MS)||\
                                                                 ((__VALUE__) == FL_IWDT_PERIOD_500MS)||\
                                                                 ((__VALUE__) == FL_IWDT_PERIOD_1000MS)||\
                                                                 ((__VALUE__) == FL_IWDT_PERIOD_2000MS)||\
                                                                 ((__VALUE__) == FL_IWDT_PERIOD_4000MS)||\
                                                                 ((__VALUE__) == FL_IWDT_PERIOD_8000MS)||\
                                                                 ((__VALUE__) == FL_IWDT_PERIOD_16000MS))
/**
  * @}
  */
  
/** @addtogroup IWDT_FL_EF_Init
  * @{
  */ 
  
/**
  * @brief	��λIWDT����
  *
  * @note   �˺���ֻ����������ǰ��λ���裬��ΪIWDT�����󲻿��Թر�
  *
  * @param	������ڵ�ַ
  *
  * @retval	���ش���״̬������ֵ��
  *       	-PASS ����Ĵ���ֵ�ָ���λֵ
  *       	-FAIL δ�ɹ�ִ��
  */
ErrorStatus FL_IWDT_DeInit(IWDT_Type* IWDTx)
{

    assert_param(IS_IWDT_INSTANCE(IWDTx));
    
    return PASS;
}
/**
  * @brief	���� IWDT_InitStruct ��ʼ����Ӧ����ļĴ���ֵ. 
  *         
  * @note 	IWTDʹ�ܺ��޷��رգ�ֱ����һ��оƬ��λ
  *      	      
  * @param	IWDTx  ������ڵ�ַ
  * @param 	IWDT_InitStruct �� @ref FL_IWDT_InitTypeDef�ṹ�壬������ָ��IWDT�����������Ϣ	  
  *         
  * @retval	ErrorStatusö��ֵ	
  *			-FAIL ���ù��̷�������        
  *			-PASS ���óɹ�        
  */
ErrorStatus FL_IWDT_Init(IWDT_Type* IWDTx,FL_IWDT_InitTypeDef *IWDT_InitStruct)
{
    ErrorStatus status = PASS;

    /* ��ڲ������ */
    assert_param(IS_IWDT_INSTANCE(IWDTx));
    assert_param(IS_FL_IWDT_WINDOWSVEL(IWDT_InitStruct->iwdtWindows));
    assert_param(IS_FL_IWDT_OVERFLOWPERIOD(IWDT_InitStruct->overflowPeriod));
    /* ��������ʱ�� */
	FL_RCC_EnableGroup1BusClock(FL_RCC_GROUP1_BUSCLK_IWDT);
    /* ���ö������Ź�������� */
    FL_IWDT_SetPeriod(IWDTx,IWDT_InitStruct->overflowPeriod);
    /* ���ö������Ź��幷����*/
    FL_IWDT_WriteWindow(IWDTx,IWDT_InitStruct->iwdtWindows);
    /* �������Ź� */
    FL_IWDT_ReloadCounter(IWDTx);
    
    return status;
}
/**
  * @brief	���� IWDT_InitStruct ΪĬ������
  * @param 	IWDT_InitStruct ָ����Ҫ��ֵ����ΪĬ�����õĽṹ�� @ref LL_IWDT_InitTypeDef �ṹ�� 
  *         
  * @retval	None
  */
void FL_IWDT_StructInit(FL_IWDT_InitTypeDef *IWDT_InitStruct)
{
    /* Ĭ�ϲ�ʹ�ô��� */
    IWDT_InitStruct->iwdtWindows    = 0;
    /*����ʱ��*/
    IWDT_InitStruct->overflowPeriod = FL_IWDT_PERIOD_500MS;
}
/** 
  *@} 
  */
  
/** 
  *@} 
  */
/*************************************************************END OF FILE************************************************************/
