/**
  *******************************************************************************************************
  * @file    fm33lc0xx_fl_bstim32.c
  * @author  FMSH Application Team
  * @brief   Src file of BSTIM32 FL Module
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
#include "fm33lc0xx_fl_rmu.h"
#include "fm33lc0xx_fl_bstim32.h"
#include "fm33_assert.h"

/** @addtogroup FM33LC0xx_FL_Driver_BSTIM
  * @{
  */
  
/** @addtogroup Private_Macros
  * @{
  */
#define         IS_FL_BSTIM32_INSTANCE(INSTANCE)                    ((INSTANCE) == BSTIM32)

#define         IS_FL_BSTIM32_PSC(__VALUE__)                        ((__VALUE__) <= 0xFFFFFFFF)

#define         IS_FL_BSTIM32_AUTORELOAD(__VALUE__)                 ((__VALUE__) <= 0xFFFFFFFF)

#define         IS_FL_BSTIM32_AUTORELOAD_MODE(__VALUE__)           (((__VALUE__) == ENABLE)||\
                                                                    ((__VALUE__) == DISABLE))

#define         IS_FL_BSTIM32_CLOCK_SRC(__VALUE__)                 (((__VALUE__) == FL_RCC_BSTIM32_CLK_SOURCE_APB1CLK)||\
                                                                    ((__VALUE__) == FL_RCC_BSTIM32_CLK_SOURCE_LSCLK)||\
                                                                    ((__VALUE__) == FL_RCC_BSTIM32_CLK_SOURCE_RCLP)||\
                                                                    ((__VALUE__) == FL_RCC_BSTIM32_CLK_SOURCE_RCMF_PSC))

/**
  * @}
  */
  
/** @addtogroup BSTIM32_FL_EF_Init
  * @{
  */ 
  
/**
  * @brief  ????????????BSTIM32?????????.
  * @param  BSTIM32x 
  * @retval ErrorStatus?????????:
  *       	-PASS ?????????????????????????????????
  *       	-FAIL ???????????????
  */
ErrorStatus FL_BSTIM32_DeInit(BSTIM32_Type *BSTIM32x)
{  
    assert_param(IS_FL_BSTIM32_INSTANCE(BSTIM32x));
    
    /* ?????????????????? */
    FL_RCC_EnablePeripheralReset();
    
    /* ????????????????????? */
    FL_RCC_EnableResetAPB2Peripheral(FL_RCC_RSTAPB_BSTIM32);
    FL_RCC_EnableResetAPB2Peripheral(FL_RCC_RSTAPB_BSTIM32);
    
    /* ?????????????????????????????????????????? */
    FL_RCC_DisableGroup4BusClock(FL_RCC_GROUP4_BUSCLK_BSTIM32);
    FL_RCC_DisableGroup2OperationClock(FL_RCC_GROUP2_OPCLK_BSTIM32);
    
    /* ?????????????????? */
    FL_RCC_DisablePeripheralReset();
    
    return PASS;
}
/**
  * @brief  ?????? BSTIM32_InitStruct ???????????????????????????????????????????????????????????????.
  * @param  BSTIMx BSTIMx 
  * @param  BSTIM32_InitStruct ???????????? @ref FL_BSTIM32_InitTypeDef ?????????
  *         ??????????????????????????????????????????.
  * @retval ErrorStatus?????????	
  *			-FAIL ????????????????????????        
  *			-PASS ????????????  
  */
ErrorStatus FL_BSTIM32_Init(BSTIM32_Type *BSTIM32x, FL_BSTIM32_InitTypeDef *initStruct)
{
    /* ???????????? */
    assert_param(IS_FL_BSTIM32_INSTANCE(BSTIM32x));
    assert_param(IS_FL_BSTIM32_CLOCK_SRC(initStruct->clockSource));
    assert_param(IS_FL_BSTIM32_PSC(initStruct->prescaler)); 
    assert_param(IS_FL_BSTIM32_AUTORELOAD(initStruct->autoReload));
    assert_param(IS_FL_BSTIM32_AUTORELOAD_MODE(initStruct->autoReloadState));
      
    /* ???????????? */                                                          
    FL_RCC_EnableGroup4BusClock(FL_RCC_GROUP4_BUSCLK_BSTIM32);
    
    /* ??????????????? */
    FL_RCC_SetBSTIM32ClockSource(initStruct->clockSource);
    FL_RCC_EnableGroup2OperationClock(FL_RCC_GROUP2_OPCLK_BSTIM32);
    
    /* ???????????? */
    FL_BSTIM32_WritePrescaler(BSTIM32x, initStruct->prescaler);
    
    /* ?????????????????? */
    FL_BSTIM32_EnableUpdateEvent(BSTIM32x);
    FL_BSTIM32_WriteAutoReload(BSTIM32x, initStruct->autoReload);
    
    if(initStruct->autoReloadState == ENABLE)
    {
        FL_BSTIM32_EnableARRPreload(BSTIM32x);
    }
    else
    {
        FL_BSTIM32_DisableARRPreload(BSTIM32x);
    }
    FL_BSTIM32_GenerateUpdateEvent(BSTIM32x);
    
    return PASS;
}

/**
  * @brief	?????? BSTIM32_InitStruct ???????????????
  * @param 	BSTIM32_InitStruct ??????????????????????????????????????????????????? @ref FL_BSTIM_InitTypeDef ?????????  
  *         
  * @retval	None
  */
void FL_BSTIM32_StructInit(FL_BSTIM32_InitTypeDef *initStruct)
{
    initStruct->prescaler         = 0;
    initStruct->autoReload        = 0xFFFFFFFF;    
    initStruct->autoReloadState   = ENABLE;
    initStruct->clockSource       = FL_RCC_BSTIM32_CLK_SOURCE_APB1CLK;
}

/**
  * @}
  */
  
/**
  * @}
  */


/*************************(C) COPYRIGHT Fudan Microelectronics **** END OF FILE*************************/
