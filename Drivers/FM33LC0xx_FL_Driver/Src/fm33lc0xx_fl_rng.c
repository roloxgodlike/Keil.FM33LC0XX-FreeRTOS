/**
  *******************************************************************************************************
  * @file    fm33lc0xx_fl_rng.c
  * @author  FMSH Application Team
  * @brief   Src file of RNG FL Module
  *******************************************************************************************************
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
  *******************************************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "fm33lc0xx_fl_rcc.h"
#include "fm33lc0xx_fl_rng.h"
#include "fm33_assert.h"
/** @addtogroup FM33LC0xx_FL_Driver_RNG
  * @{
  */
  
/** @addtogroup Private_Macros
* @{
*/
#define         IS_FL_RNG_INSTANCE(INTANCE)                         ((INTANCE) == RNG)

#define         IS_FL_RNG_CLOCK_PRESCALER(__VALUE__)               (((__VALUE__) == FL_RCC_RNG_PSC_DIV1)||\
                                                                     ((__VALUE__) == FL_RCC_RNG_PSC_DIV2)||\
                                                                     ((__VALUE__) == FL_RCC_RNG_PSC_DIV4)||\
                                                                     ((__VALUE__) == FL_RCC_RNG_PSC_DIV8)||\
                                                                     ((__VALUE__) == FL_RCC_RNG_PSC_DIV16)||\
                                                                     ((__VALUE__) == FL_RCC_RNG_PSC_DIV32))
/** 
  *@} 
  */ 

/** @addtogroup RNG_FL_EF_Init
  * @{
  */   
/**
  * @brief  ��λ��ӦRNG�Ĵ���.
  * @param  RNGx 
  * @retval ErrorStatusö��ֵ:
  *       	-PASS ����Ĵ���ֵ�ָ���λֵ
  *       	-FAIL δ�ɹ�ִ��
  */
ErrorStatus FL_RNG_DeInit(RNG_Type *RNGx)
{
    assert_param(IS_FL_RNG_INSTANCE(RNGx));
    
    /* ʹ�����踴λ */
    FL_RCC_EnablePeripheralReset();
    /* ��λ����Ĵ��� */
    FL_RCC_EnableResetAPB2Peripheral(FL_RCC_RSTAPB_RNG);
    FL_RCC_DisableResetAPB2Peripheral(FL_RCC_RSTAPB_RNG);
    /* �ر���������ʼʱ�Ӻ͹���ʱ�� */
    FL_RCC_DisableGroup2BusClock(FL_RCC_GROUP2_BUSCLK_RNG);
    /* �������踴λ */
    FL_RCC_DisablePeripheralReset();
    return PASS;
}
/**
  * @brief  ���� InitStruct ��������Ϣ��ʼ����Ӧ������ڵ�ַ�ļĴ���ֵ.
  * @param  RNGx  
  * @param  InitStruct ָ��һ�� @ref FL_RNG_InitTypeDef �ṹ��
  *         ���а�������������������Ϣ.
  * @note   RNGʹ��RCHFĬ�ϵ�8M��Ϊʱ�����룬��2��Ƶ��4M�ṩ��RNG
  * @retval ErrorStatusö��ֵ	
  *			-FAIL ���ù��̷�������        
  *			-PASS ���óɹ�  
  */
ErrorStatus FL_RNG_Init(RNG_Type* RNGx, FL_RNG_InitTypeDef *initStruct)
{
    
    assert_param(IS_FL_RNG_INSTANCE(RNGx));
    assert_param(IS_FL_RNG_CLOCK_PRESCALER(initStruct->prescaler));
    /* RNG ʹ��RCHF��Ϊ����ʱ����˱���ȷ��RCHFʹ��*/
    if(FL_RCC_RCHF_IsEnabled() != SET)
    {
        FL_RCC_RCHF_Enable();
    }
    /* RNG ����ʱ��ʹ�ܣ���Ϊ�õ��������Ҫ��ģ�⹦��*/
    FL_RCC_EnableGroup1BusClock(FL_RCC_GROUP1_BUSCLK_ANAC);
    FL_RCC_EnableGroup2BusClock(FL_RCC_GROUP2_BUSCLK_RNG);
    /* RNG ����ʱ��Ԥ��Ƶ*/
    FL_RCC_SetRNGPrescaler(initStruct->prescaler);
    /* RNG ����ʱ��ʹ��*/
    FL_RCC_EnableGroup2OperationClock(FL_RCC_GROUP2_OPCLK_RNG);
    
    return PASS;
}
/**
  * @brief	���� InitStruct ΪĬ������
  * @param 	InitStruct ָ����Ҫ��ֵ����ΪĬ�����õĽṹ�� @ref FL_RNG_InitTypeDef �ṹ��  
  *         
  * @retval	None
  */

void FL_RNG_StructInit(FL_RNG_InitTypeDef *initStruct)
{
    initStruct->prescaler = FL_RCC_RNG_PSC_DIV2;
}

/**
  * @brief  ��ȡһ�������
  *
  * @param  None
  *
  * @note   �������ֵΪ0xFFFFFFFF ��˵�����������ʧ�ܣ����û����˺����ķ���ֵ��ʧ��ʱ��������   
  *
  * @retval �ɹ������������ʧ�ܷ���0xFFFFFFFF
  *                
  */
uint32_t GetRandomNumber(void)
{
    uint32_t rn32;
    FL_RNG_ClearFlag_RandomFail(RNG);
    FL_RNG_Enable(RNGCTL);
    __NOP();
    __NOP();
    __NOP();
    FL_RNG_Disable(RNGCTL);
    rn32 = FL_RNG_ReadData(RNG);
    if(FL_RNG_IsActiveFlag_RandomFail(RNG))
    {
        FL_RNG_ClearFlag_RandomFail(RNG);
        return 0xFFFFFFFF;
    }
    return rn32;
   
}

/**
  * @brief  ��ȡCRC32
  *
  * @param  dataIn�������������
  *
  * @note   None   
  *
  * @retval �ɹ�����CRC32��ʧ�ܷ���0xFFFFFFFF
  *                
  */
uint32_t GetCrc32(uint32_t dataIn)
{
    uint32_t i=0;
	uint32_t crc32=0;
    FL_RNG_CRC_WriteData(RNG,dataIn);
    FL_RNG_ClearFlag_CRCComplete(RNG);
    FL_RNG_CRC_Enable(RNG);
    while (0 == FL_RNG_IsActiveFlag_CRCComplete(RNG))
    {
        i++;
        if (i > 600)
            break;
    }
    if(i >= 600)
    {
        FL_RNG_ClearFlag_CRCComplete(RNG);
        FL_RNG_Disable(RNGCTL);
        return 0xFFFFFFFF;
    }
    FL_RNG_ClearFlag_CRCComplete(RNG);
    crc32 = FL_RNG_ReadData(RNG);
    FL_RNG_Disable(RNGCTL);
    return crc32;
}

/** 
  *@} 
  */

/** 
  *@} 
  */
/*************************************************************END OF FILE************************************************************/
