/**
  *******************************************************************************************************
  * @file    fm33lc0xx_fl_opa.c
  * @author  FMSH Application Team
  * @brief   Src file of OPA FL Module
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
#include "fm33lc0xx_fl_rcc.h"
#include "fm33lc0xx_fl_rmu.h"
#include "fm33lc0xx_fl_opa.h"
#include "fm33_assert.h"
    
/** @addtogroup FM33LC0xx_FL_Driver_OPA
  * @{
  */
/** @addtogroup Private_Macros
  * @{
  */
#define         IS_OPA_ALL_INSTANCE(INTENCE)               (((INTENCE) == OPA1)||\
                                                            ((INTENCE) == OPA2))
                                                            
#define         IS_FL_OPA_INP_CHANNAL(__VALUE__)           (((__VALUE__) == FL_OPA_INP_SOURCE_INP1)||\
                                                            ((__VALUE__) == FL_OPA_INP_SOURCE_INP2))


#define         IS_FL_OPA_INN_CHANNAL(__VALUE__)           (((__VALUE__) == FL_OPA_INN_SOURCE_INN1)||\
                                                            ((__VALUE__) == FL_OPA_INN_SOURCE_INN2)||\
                                                            ((__VALUE__) == FL_OPA_INN_SOURCE_VREF)||\
                                                            ((__VALUE__) == FL_OPA_INN_SOURCE_THREE_QUARTERS_VREF)||\
                                                            ((__VALUE__) == FL_OPA_INN_SOURCE_HALF_VREF)||\
                                                            ((__VALUE__) == FL_OPA_INN_SOURCE_ONE_QUARTER_VREF)||\
                                                            ((__VALUE__) == FL_OPA_INN_SOURCE_ONE_EIGHTH_VREF))
                                                            
#define         IS_FL_OPA_MODE(__VALUE__)                  (((__VALUE__) == FL_OPA_MODE_STANDLONE)||\
                                                            ((__VALUE__) == FL_OPA_MODE_COMPARATOR)||\
                                                            ((__VALUE__) == FL_OPA_MODE_PGA)||\
                                                            ((__VALUE__) == FL_OPA_MODE_BUFFER))                                                            
                                                                           
#define         IS_FL_OPA_DIGITALFILTER(__VALUE__)         (((__VALUE__) == DISABLE)||\
                                                            ((__VALUE__) == ENABLE))
                                                             
#define         IS_FL_OPA_NEGTIVE_TO_PIN(__VALUE__)        (((__VALUE__) == DISABLE)||\
                                                            ((__VALUE__) == ENABLE)) 

#define         IS_FL_OPA_LOW_POWER_MODE(__VALUE__)        (((__VALUE__) == DISABLE)||\
                                                            ((__VALUE__) == ENABLE))                                                                                               
                                                            
#define         IS_FL_OPA_GAIN(__VALUE__)                  (((__VALUE__) == FL_OPA_GAIN_X2)||\
                                                            ((__VALUE__) == FL_OPA_GAIN_X4)||\
                                                            ((__VALUE__) == FL_OPA_GAIN_X8)||\
                                                            ((__VALUE__) == FL_OPA_GAIN_X16))                                                            
                                                            
/**
  * @}
  */
/**
  * @brief	复位OPA 外设寄存器值为复位值
  * @param	外设入口地址
  * @retval	返回错误状态，可能值：
  *       	-PASS 外设寄存器值恢复复位值
  *       	-FAIL 未成功执行
  */
ErrorStatus FL_OPA_DeInit(OPA_Type *OPAx)
{
    /* 入口参数合法性断言 */
    assert_param(IS_OPA_ALL_INSTANCE(OPAx));
    
	/* 使能外设复位 */
    FL_RCC_EnablePeripheralReset();
    /* 复位外设寄存器 */
    FL_RCC_EnableResetAPB2Peripheral(FL_RCC_RSTAPB_OPA);
    FL_RCC_DisableResetAPB2Peripheral(FL_RCC_RSTAPB_OPA);
    /* 关闭外设总线时钟和工作时钟 */
    FL_RCC_DisableGroup1BusClock(FL_RCC_GROUP1_BUSCLK_ANAC);
    /* 锁定外设复位 */
    FL_RCC_DisablePeripheralReset();
    return PASS;
}                                                            
                                                            
ErrorStatus FL_OPA_Init(OPA_Type *OPAx, FL_OPA_InitTypeDef *initStruct)
{
    ErrorStatus status = PASS;

    /* 入口参数检查 */
    assert_param(IS_OPA_ALL_INSTANCE(OPAx));
    assert_param(IS_FL_OPA_INP_CHANNAL(initStruct->INP));
    assert_param(IS_FL_OPA_INN_CHANNAL(initStruct->INN));
    assert_param(IS_FL_OPA_MODE(initStruct->mode));
    assert_param(IS_FL_OPA_DIGITALFILTER(initStruct->digitalFilter));
    assert_param(IS_FL_OPA_NEGTIVE_TO_PIN(initStruct->negtiveToPin));
    assert_param(IS_FL_OPA_LOW_POWER_MODE(initStruct->lowPowermode));
    assert_param(IS_FL_OPA_GAIN(initStruct->gain));
    /*配置模式*/
    FL_OPA_SetMode(OPAx,initStruct->mode);
    /*配置反向输入*/
    FL_OPA_SetINNSource(OPAx,initStruct->INN);
    /*配置正向输入*/
    FL_OPA_SetINPSource(OPAx,initStruct->INP);
    /*配置低功耗模式*/
    if(initStruct->lowPowermode == ENABLE)
    {
       FL_OPA_EnableLowPowerMode(OPAx); 
    }
    
    if(initStruct->mode == FL_OPA_MODE_PGA)
    {
        /*配置PGA增益*/
        FL_OPA_PGA_SetGain(OPAx,initStruct->gain);
        if(initStruct->negtiveToPin == ENABLE)
        {
            /*配置PGA模式下反向输入端接到PIN*/
            FL_OPA_PGA_EnableINNConnectToPin(OPAx);
        }
    }
    return status;
}                                                            
      /**
  * @brief	FL_OPA_InitTypeDef 为默认配置
  * @param 	FL_OPA_InitTypeDef 指向需要将值设置为默认配置的结构体 @ref FL_OPA_InitTypeDef structure 
  *         结构体
  * @retval	None
  */

void FL_OPA_StructInit(FL_OPA_InitTypeDef *initStruct)
{
   initStruct->INP              = FL_OPA_INP_SOURCE_INP1;               
   initStruct->INN              = FL_OPA_INN_SOURCE_INN1;     
   initStruct->mode             = FL_OPA_MODE_STANDLONE;                   
   initStruct->digitalFilter    = DISABLE;     
   initStruct->negtiveToPin     = DISABLE;     
   initStruct->gain             = FL_OPA_GAIN_X2;   
   initStruct->lowPowermode     = DISABLE; 
}
/** 
  *@} 
  */
/** 
  *@} 
  */
/*************************************************************END OF FILE************************************************************/    
