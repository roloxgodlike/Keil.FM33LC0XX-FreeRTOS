/**
  *******************************************************************************************************
  * @file    fm33lc0xx_fl_divas.c
  * @author  FMSH Application Team
  * @brief   Src file of DIVAS FL Module
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
#include "fm33lc0xx_fl_divas.h"
#include "fm33_assert.h"

/** @addtogroup FM33LC0XX_FL_Driver_DIVAS
  * @{
  */

/* Private macros ------------------------------------------------------------*/
/** @addtogroup DIVAS_FL_Private_Macros
  * @{
  */  
#define         IS_DIVAS_ALL_INSTANCE(INTENCE)              ((INTENCE) == DIV)
                                                            
#define         IS_FL_DIVAS_DIVISOR(__VALUE__)              (((__VALUE__) != 0))
    
/**
  * @}
  */

/** @addtogroup DIVAS_FL_EF_Init
  * @{
  */ 
  
/**
  * @brief  ��λ��ӦHDIV���ƼĴ���.
  * @param  DIVx HDIV Port
  * @retval ErrorStatusö��ֵ:
  *       	-PASS ����Ĵ���ֵ�ָ���λֵ
  *       	-FAIL δ�ɹ�ִ��
  */
ErrorStatus FL_DIVAS_DeInit(DIV_Type *DIVx)
{
    /* ��ڲ������ */
    assert_param(IS_DIVAS_ALL_INSTANCE(DIVx));
    /* ���踴λʹ�� */
    FL_RCC_EnablePeripheralReset();   
    /* �ָ��Ĵ���ֵΪĬ��ֵ */
    FL_RCC_EnableResetAPB2Peripheral(FL_RCC_RSTAPB_DIVAS);
    FL_RCC_DisableResetAPB2Peripheral(FL_RCC_RSTAPB_DIVAS);
	  /* �ر�����ʱ�� */
    FL_RCC_DisableGroup2BusClock(FL_RCC_GROUP2_BUSCLK_HDIV);
    /* �������踴λ���� */
	FL_RCC_DisablePeripheralReset();
    return PASS;
}

/**
  * @brief  ���� ��ʼ����Ӧ����DIVAS.
  *
  * @param  DIVx HDIV Port
  *        
  * @retval ErrorStatusö��ֵ	
  *			-FAIL ���ù��̷�������        
  *			-PASS LPUART���óɹ�  
  */
ErrorStatus FL_DIVAS_Init(DIV_Type *DIVx)
{
    /* ��ڲ������ */
    assert_param(IS_DIVAS_ALL_INSTANCE(DIVx));
    /* ʹ��ʱ������ */
    FL_RCC_EnableGroup2BusClock(FL_RCC_GROUP2_BUSCLK_HDIV);
    return PASS;
}
/**
  * @brief Ӳ�����������㺯��	
  *
  * @param 	DIVx HDIV Port 
  *
  * @param 	32λ�з��ű�����
  *
  * @param 	32λ�з��ų�����ע�ⲻ��Ϊ0
  *
  * @param 	�����̵ĵ�ַ
  *
  * @param 	���������ĵ�ַ
  *         
  * @retval	���ط�0 ��ʾ�������
  */
uint32_t FL_DIVAS_Hdiv_Calculation(DIV_Type *DIVx,int32_t DivisorEnd,int16_t Divisor,int32_t* Quotient, int16_t* Residue)
{
    uint32_t  TimeOut ;
	
    FL_DIV_WriteDividend_S32(DIVx,DivisorEnd);
    FL_DIV_WriteDivisor_S16(DIVx,Divisor);
    
    if(FL_DIV_IsActiveFlag_DividedZero(DIVx))
    {
        /*����Ϊ0 */
        *Quotient = 0;
        *Residue  = 0;
        return 1;
        
    }
    TimeOut = FL_DIVAS_SR_BUSY_TIMEOUT;
    while(FL_DIV_IsActiveFlag_Busy(DIVx))
    {
        TimeOut --;
        if(TimeOut<=0)
        {
            /* ���㳬ʱ*/
            *Quotient = 0;
            *Residue  = 0;
            return 3;
        }
            
    }
    *Quotient = FL_DIV_ReadQuotient_S32(DIVx);
    *Residue  = FL_DIV_ReadResidue_S16(DIVx);
    return 0;
}


/**
  * @}
  */

/**
  * @}
  */
/*************************************************************END OF FILE************************************************************/


