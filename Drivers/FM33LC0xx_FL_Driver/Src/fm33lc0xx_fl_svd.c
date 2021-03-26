/**
  ****************************************************************************************************
  * @file    fm33lc0xx_fl_svd.c
  * @author  FMSH Application Team
  * @brief   Src file of SVD FL Module
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
#include "fm33lc0xx_fl_rmu.h"
#include "fm33lc0xx_fl_svd.h"
#include "fm33_assert.h"

/** @addtogroup FM33LC0xx_FL_Driver_SVD
  * @{
  */
  
/* Private macros ------------------------------------------------------------*/
/** @addtogroup UART_FL_Private_Macros
  * @{
  */
#define         IS_SVD_INSTANCE(INSTANCE)                  (((INSTANCE) == SVD))

#define         IS_FL_SVD_WARNING_THRESHOLD_LEVEL(__VALUE__)        (((__VALUE__) == FL_SVD_WARNING_THRESHOLD_GROUP0)||\
                                                                     ((__VALUE__) == FL_SVD_WARNING_THRESHOLD_GROUP1)||\
                                                                     ((__VALUE__) == FL_SVD_WARNING_THRESHOLD_GROUP2)||\
                                                                     ((__VALUE__) == FL_SVD_WARNING_THRESHOLD_GROUP3)||\
                                                                     ((__VALUE__) == FL_SVD_WARNING_THRESHOLD_GROUP4)||\
                                                                     ((__VALUE__) == FL_SVD_WARNING_THRESHOLD_GROUP5)||\
                                                                     ((__VALUE__) == FL_SVD_WARNING_THRESHOLD_GROUP6)||\
                                                                     ((__VALUE__) == FL_SVD_WARNING_THRESHOLD_GROUP7)||\
                                                                     ((__VALUE__) == FL_SVD_WARNING_THRESHOLD_GROUP8)||\
                                                                     ((__VALUE__) == FL_SVD_WARNING_THRESHOLD_GROUP9)||\
                                                                     ((__VALUE__) == FL_SVD_WARNING_THRESHOLD_GROUP10)||\
                                                                     ((__VALUE__) == FL_SVD_WARNING_THRESHOLD_GROUP11)||\
                                                                     ((__VALUE__) == FL_SVD_WARNING_THRESHOLD_GROUP12)||\
                                                                     ((__VALUE__) == FL_SVD_WARNING_THRESHOLD_GROUP13)||\
                                                                     ((__VALUE__) == FL_SVD_WARNING_THRESHOLD_GROUP14)||\
                                                                     ((__VALUE__) == FL_SVD_WARNING_THRESHOLD_GROUP15))
                                                                     
#define         IS_FL_SVD_SVSCONFIG(__VALUE__)             (((__VALUE__) == DISABLE)||\
                                                            ((__VALUE__) == ENABLE))
                                                             
#define         IS_FL_SVD_REFERENCE_VOLTAGE(__VALUE__)     (((__VALUE__) == FL_SVD_REFERENCE_1P2V)||\
                                                            ((__VALUE__) == FL_SVD_REFERENCE_1P1V)||\
                                                            ((__VALUE__) == FL_SVD_REFERENCE_1P0V))
                                                          
#define         IS_FL_SVD_DIGITAL_FILTER(__VALUE__)        (((__VALUE__) == DISABLE)||\
                                                            ((__VALUE__) == ENABLE))                                                             
/** 
  *@} 
  */
/** @addtogroup SVD_FL_EF_Init
  * @{
  */   
/**
  * @brief	复位SVD 外设寄存器值为复位值
  * @param	外设入口地址
  * @retval	返回错误状态，可能值：
  *       	-PASS 外设寄存器值恢复复位值
  *       	-FAIL 未成功执行
  */
ErrorStatus FL_SVD_DeInit(SVD_Type* SVDx)
{
    ErrorStatus status = PASS;

    /* 入口参数合法性断言 */
    assert_param(IS_SVD_INSTANCE(SVDx));
	SVDx->CFGR  = 0x00000C08U;
    SVDx->CR    = 0x00000000U; 
    SVDx->IER   = 0x00000000U;
    SVDx->VSR   = 0x00000004U;
    
    return (status);
}


/**
  * @brief	根据 SVD_InitStruct 结构体包含的配置信息配置PMU寄存器 
  *         
  * @note 	为更好的睡眠功耗用户可能需要根据实际应用，调用 @ref fm33lc0xx_ll_RCC.h中的其他接口
  *      	来完成睡眠前的模式配置，包括睡眠行为和唤醒后的行为      
  * @param	PMUx  外设入口地址
  * @param 	SVD_InitStruct 指向一个 @ref SVD_InitTypeDef 类型的结构体，它包含指定LPUART外设的配置信息	  
  *         
  * @retval	ErrorStatus枚举值	
  *			-FAIL 配置过程发生错误        
  *			-PASS LPUART配置成功        
  */
ErrorStatus FL_SVD_Init(SVD_Type* SVDx, FL_SVD_InitTypeDef *initStruct)
{
    ErrorStatus status = FAIL;
    
	/* 参数合法性检查 */
    assert_param(IS_SVD_INSTANCE(SVDx));
    assert_param(IS_FL_SVD_WARNING_THRESHOLD_LEVEL(initStruct->warningThreshold));
    assert_param(IS_FL_SVD_SVSCONFIG(initStruct->SVSChannel));
    assert_param(IS_FL_SVD_REFERENCE_VOLTAGE(initStruct->referenceVoltage));
    assert_param(IS_FL_SVD_DIGITAL_FILTER(initStruct->digitalFilter));
    
    /* 使能工作时钟 */
    FL_RCC_EnableGroup1BusClock(FL_RCC_GROUP1_BUSCLK_ANAC);
    
    /* 检测阈值 */
    FL_SVD_SetWarningThreshold(SVDx, initStruct->warningThreshold);
   
    /* 参考电压通道选择 */
    FL_SVD_EnableReference(SVD, initStruct->referenceVoltage);
    
    /* 外部通道配置 */
    if(initStruct->SVSChannel == ENABLE)
    {
        FL_SVD_EnableSVSChannel(SVDx);
    }
    else
    {
        FL_SVD_DisableSVSChannel(SVDx);
    }
    
    /* 数字滤波配置 */
    if(initStruct->digitalFilter == ENABLE)
    {
        FL_SVD_EnableDigitalFilter(SVDx);
    }
    else
    {
        FL_SVD_DisableDigitalFilter(SVDx);
    }
    
    status = PASS;
    return status;
}


/**
  * @brief	SVD_InitStruct 为默认配置
  * @param 	SVD_InitStruct 指向需要将值设置为默认配置的结构体 @ref SVD_InitTypeDef structure 
  *         结构体
  * @retval	None
  */

void FL_SVD_StructInit(FL_SVD_InitTypeDef *initStruct)
{
    
    initStruct->SVSChannel          = DISABLE;
    initStruct->digitalFilter       = ENABLE;
    initStruct->referenceVoltage    = FL_SVD_REFERENCE_1P0V;
    initStruct->warningThreshold    = FL_SVD_WARNING_THRESHOLD_GROUP0;

}
/** 
  *@} 
  */
/** 
  *@} 
  */
/*********************** (C) COPYRIGHT Fudan Microelectronics *****END OF FILE************************/

