/**
  *******************************************************************************************************
  * @file    fm33lc0xx_fl_gptim.c
  * @author  FMSH Application Team
  * @brief   Src file of GPTIM FL Module
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
#include "fm33lc0xx_fl_gptim.h"
#include "fm33_assert.h"

/** @addtogroup FM33LC0xx_FL_Driver_GPTIM
  * @{
  */
  
  
/** @addtogroup TIM_FL_Private_Macros
  * @{
  */


#define IS_GPTIM_INSTANCE(TIMx)                     (((TIMx) == GPTIM0) || \
                                                     ((TIMx) == GPTIM1))
                                                     

#define IS_FL_GPTIM_COUNTERMODE(__VALUE__)          (((__VALUE__) == FL_GPTIM_COUNTER_DIR_UP) || \
                                                     ((__VALUE__) == FL_GPTIM_COUNTER_DIR_DOWN) || \
                                                     ((__VALUE__) == FL_GPTIM_COUNTER_ALIGNED_EDGE) || \
                                                     ((__VALUE__) == FL_GPTIM_COUNTER_ALIGNED_CENTER_UP) || \
                                                     ((__VALUE__) == FL_GPTIM_COUNTER_ALIGNED_CENTER_DOWN) || \
                                                     ((__VALUE__) == FL_GPTIM_COUNTER_ALIGNED_CENTER_UP_DOWN))

#define IS_FL_GPTIM_CLOCKDIVISION(__VALUE__)        (((__VALUE__) == FL_GPTIM_CLK_DIVISION_DIV1) || \
                                                     ((__VALUE__) == FL_GPTIM_CLK_DIVISION_DIV2) || \
                                                     ((__VALUE__) == FL_GPTIM_CLK_DIVISION_DIV4))


#define IS_FL_GPTIM_CHANNEL_MODE(__VALUE__)         (((__VALUE__) == FL_GPTIM_CHANNEL_MODE_OUTPUT) || \
                                                     ((__VALUE__) == FL_GPTIM_CHANNEL_MODE_INPUT_NORMAL) || \
                                                     ((__VALUE__) == FL_GPTIM_CHANNEL_MODE_INPUT_CROSSOVER) || \
                                                     ((__VALUE__) == FL_GPTIM_CHANNEL_MODE_INPUT_TRC))
                                      
#define IS_FL_GPTIM_IC_FILTER(__VALUE__)            (((__VALUE__) == FL_GPTIM_IC_FILTER_DIV1) || \
                                                     ((__VALUE__) == FL_GPTIM_IC_FILTER_DIV1_N2) || \
                                                     ((__VALUE__) == FL_GPTIM_IC_FILTER_DIV1_N4) || \
                                                     ((__VALUE__) == FL_GPTIM_IC_FILTER_DIV1_N8) || \
                                                     ((__VALUE__) == FL_GPTIM_IC_FILTER_DIV2_N6) || \
                                                     ((__VALUE__) == FL_GPTIM_IC_FILTER_DIV2_N8) || \
                                                     ((__VALUE__) == FL_GPTIM_IC_FILTER_DIV4_N6) || \
                                                     ((__VALUE__) == FL_GPTIM_IC_FILTER_DIV4_N8) || \
                                                     ((__VALUE__) == FL_GPTIM_IC_FILTER_DIV8_N6) || \
                                                     ((__VALUE__) == FL_GPTIM_IC_FILTER_DIV8_N8) || \
                                                     ((__VALUE__) == FL_GPTIM_IC_FILTER_DIV16_N5) || \
                                                     ((__VALUE__) == FL_GPTIM_IC_FILTER_DIV16_N6) || \
                                                     ((__VALUE__) == FL_GPTIM_IC_FILTER_DIV16_N8) || \
                                                     ((__VALUE__) == FL_GPTIM_IC_FILTER_DIV32_N5) || \
                                                     ((__VALUE__) == FL_GPTIM_IC_FILTER_DIV32_N6) || \
                                                     ((__VALUE__) == FL_GPTIM_IC_FILTER_DIV32_N8))

#define IS_FL_GPTIM_CHANNEL(__VALUE__)              (((__VALUE__) == FL_GPTIM_CHANNEL_1)\
                                                    || ((__VALUE__) == FL_GPTIM_CHANNEL_2)\
                                                    || ((__VALUE__) == FL_GPTIM_CHANNEL_3)\
                                                    || ((__VALUE__) == FL_GPTIM_CHANNEL_4))
                                        
                                        
                                        
#define IS_FL_GPTIM_SLAVE_MODE(__VALUE__)           (((__VALUE__) == FL_GPTIM_SLAVE_MODE_PROHIBITED)\
                                                    || ((__VALUE__) == FL_GPTIM_SLAVE_MODE_ENCODER_X2_TI1)\
                                                    || ((__VALUE__) == FL_GPTIM_SLAVE_MODE_ENCODER_X2_TI2)\
                                                    || ((__VALUE__) == FL_GPTIM_SLAVE_MODE_ENCODER_X4_TI1TI2)\
                                                    || ((__VALUE__) == FL_GPTIM_SLAVE_MODE_TRGI_RISE_RST)\
                                                    || ((__VALUE__) == FL_GPTIM_SLAVE_MODE_TRGI_HIGH_RUN)\
                                                    || ((__VALUE__) == FL_GPTIM_SLAVE_MODE_TRGI_RISE_RUN)\
                                                    || ((__VALUE__) == FL_GPTIM_SLAVE_MODE_TRGI_CLK))


#define IS_FL_GPTIM_TRIGGER_SRC(__VALUE__)          (((__VALUE__) ==FL_GPTIM_TIM_TS_ITR0 )\
                                                        ||((__VALUE__) ==FL_GPTIM_TIM_TS_ITR1 )\
                                                        ||((__VALUE__) ==FL_GPTIM_TIM_TS_ITR2)\
                                                        ||((__VALUE__) ==FL_GPTIM_TIM_TS_ITR3)\
                                                        ||((__VALUE__) ==FL_GPTIM_TIM_TS_TI1F_ED)\
                                                        ||((__VALUE__) ==FL_GPTIM_TIM_TS_TI1FP1)\
                                                        ||((__VALUE__) ==FL_GPTIM_TIM_TS_TI2FP2)\
                                                        ||((__VALUE__) ==FL_GPTIM_TIM_TS_ETRF))    



#define IS_FL_GPTIM_ETR_FILTER(__VALUE__)           (((__VALUE__) == FL_GPTIM_ETR_FILTER_DIV1) || \
                                                     ((__VALUE__) == FL_GPTIM_ETR_FILTER_DIV1_N2) || \
                                                     ((__VALUE__) == FL_GPTIM_ETR_FILTER_DIV1_N4) || \
                                                     ((__VALUE__) == FL_GPTIM_ETR_FILTER_DIV1_N8) || \
                                                     ((__VALUE__) == FL_GPTIM_ETR_FILTER_DIV2_N6) || \
                                                     ((__VALUE__) == FL_GPTIM_ETR_FILTER_DIV2_N8) || \
                                                     ((__VALUE__) == FL_GPTIM_ETR_FILTER_DIV4_N6) || \
                                                     ((__VALUE__) == FL_GPTIM_ETR_FILTER_DIV4_N8) || \
                                                     ((__VALUE__) == FL_GPTIM_ETR_FILTER_DIV8_N6) || \
                                                     ((__VALUE__) == FL_GPTIM_ETR_FILTER_DIV8_N8) || \
                                                     ((__VALUE__) == FL_GPTIM_ETR_FILTER_DIV16_N5) || \
                                                     ((__VALUE__) == FL_GPTIM_ETR_FILTER_DIV16_N6) || \
                                                     ((__VALUE__) == FL_GPTIM_ETR_FILTER_DIV16_N8) || \
                                                     ((__VALUE__) == FL_GPTIM_ETR_FILTER_DIV32_N5) || \
                                                     ((__VALUE__) == FL_GPTIM_ETR_FILTER_DIV32_N6) || \
                                                     ((__VALUE__) == FL_GPTIM_ETR_FILTER_DIV32_N8))


#define IS_FL_GPTIM_ETR_PSC(__VALUE__)              (((__VALUE__) == FL_GPTIM_ETR_PSC_DIV1) ||\
                                                     ((__VALUE__) == FL_GPTIM_ETR_PSC_DIV2) ||\
                                                     ((__VALUE__) == FL_GPTIM_ETR_PSC_DIV4) ||\
                                                     ((__VALUE__) == FL_GPTIM_ETR_PSC_DIV8))                               

#define IS_FL_GPTIM_ETR_POLARITY(__VALUE__)         (((__VALUE__) == FL_GPTIM_ETR_POLARITY_NORMAL) || \
                                                     ((__VALUE__) == FL_GPTIM_ETR_POLARITY_INVERT)) 

                                   
                                                    
#define IS_FL_GPTIM_IC_POLARITY(__VALUE__)          (((__VALUE__) == FL_GPTIM_IC_POLARITY_NORMAL) \
                                                       || ((__VALUE__) == FL_GPTIM_IC_POLARITY_INVERT))
    
                                       
 
#define IS_FL_GPTIM_IC_PSC(__VALUE__)               (((__VALUE__) == FL_GPTIM_IC_PSC_DIV1) \
                                                     || ((__VALUE__) == FL_GPTIM_IC_PSC_DIV2) \
                                                     || ((__VALUE__) == FL_GPTIM_IC_PSC_DIV4) \
                                                     || ((__VALUE__) == FL_GPTIM_IC_PSC_DIV8))
                                                       
#define IS_FL_GPTIM_OC_POLARITY(__VALUE__)          (((__VALUE__) == FL_GPTIM_OC_POLARITY_NORMAL) \
                                                    || ((__VALUE__) == FL_GPTIM_OC_POLARITY_INVERT))

#define IS_FL_GPTIM_OC_MODE(__VALUE__)               (((__VALUE__) ==   FL_GPTIM_OC_MODE_FROZEN) \
                                                      || ((__VALUE__) == FL_GPTIM_OC_MODE_ACTIVE) \
                                                      || ((__VALUE__) == FL_GPTIM_OC_MODE_INACTIVE) \
                                                      || ((__VALUE__) == FL_GPTIM_OC_MODE_TOGGLE) \
                                                      || ((__VALUE__) == FL_GPTIM_OC_MODE_FORCED_INACTIVE) \
                                                      || ((__VALUE__) == FL_GPTIM_OC_MODE_FORCED_ACTIVE) \
                                                      || ((__VALUE__) == FL_GPTIM_OC_MODE_PWM1) \
                                                      || ((__VALUE__) == FL_GPTIM_OC_MODE_PWM2))
                                                      
#define IS_FL_GPTIM_OC_FASTMODE(__VALUE__)          (((__VALUE__) == ENABLE) \
                                                    || ((__VALUE__) == DISABLE))

#define IS_FL_GPTIM_OC_PRELOAD(__VALUE__)           (((__VALUE__) == ENABLE) \
                                                    || ((__VALUE__) == DISABLE))
                                                    
#define IS_FL_GPTIM_OC_ETR_CLEARN(__VALUE__)        (((__VALUE__) == ENABLE) \
                                                    || ((__VALUE__) == DISABLE)) 
     

#define IS_FL_GPTIM_TRIGGER_DELAY(__VALUE__)       (((__VALUE__) == DISABLE) \
                                                    || ((__VALUE__) == ENABLE))
                                                        
                                                        
#define IS_FL_GPTIM_IC_CAPTURE_STATE(__VALUE__)    (((__VALUE__) == DISABLE) \
                                                     || ((__VALUE__) == ENABLE))                                                        
/**
  * @}
  */


/* Private function prototypes -----------------------------------------------*/
/** @defgroup TIM_FL_Private_Functions TIM Private Functions
  * @{
  */
static ErrorStatus  OCConfig(GPTIM_Type *TIMx, uint32_t Channel, FL_GPTIM_OC_InitTypeDef *TIM_OC_InitStruct );

/**
  * @}
  */

/** @addtogroup TIM_FL_EF_Init
  * @{
  */

/**
  * @brief  ????????GPTIMx??????.
  * @param  GPTIMx 
  * @retval ErrorStatus??????:
  *       	-PASS ??????????????????????
  *       	-FAIL ??????????
  */
ErrorStatus FL_GPTIM_DeInit(GPTIM_Type *TIMx)
{
    ErrorStatus result = PASS;

    /* Check the parameters */
    assert_param(IS_GPTIM_INSTANCE(TIMx));
    
    /* ???????????? */
    FL_RCC_EnablePeripheralReset();
    
    if(TIMx == GPTIM0)
    {
        /* ???????????? */
        FL_RCC_EnableResetAPB1Peripheral(FL_RCC_RSTAPB_GPTIM0);
        FL_RCC_DisableResetAPB1Peripheral(FL_RCC_RSTAPB_GPTIM0);
        
        /* ???????????? */
        FL_RCC_DisableGroup4BusClock(FL_RCC_GROUP4_BUSCLK_GPTIM0);
    }
    else if(TIMx == GPTIM1)
    {
        /* ???????????? */
        FL_RCC_EnableResetAPB1Peripheral(FL_RCC_RSTAPB_GPTIM1);
        FL_RCC_DisableResetAPB1Peripheral(FL_RCC_RSTAPB_GPTIM1);
        
        /* ???????????? */
        FL_RCC_DisableGroup4BusClock(FL_RCC_GROUP4_BUSCLK_GPTIM1);
    }
    else
    {
        result = FAIL;
    }
    /* ???????????? */
    FL_RCC_DisablePeripheralReset();
    
    return result;
}
/**
  * @brief  ????????????????????????????????????.
  * @param  TIMx Timer Instance
  * @param  TIM_InitStruct ???????? @ref FL_GPTIM_InitTypeDef(??????????????)
  * @retval ErrorStatus??????	
  *			-FAIL ????????????????        
  *			-PASS ????  
  */
ErrorStatus FL_GPTIM_Init(GPTIM_Type *TIMx, FL_GPTIM_InitTypeDef *init)
{

    /* ???????? */
    assert_param(IS_GPTIM_INSTANCE(TIMx));
    assert_param(IS_FL_GPTIM_COUNTERMODE(init->counterMode));
    assert_param(IS_FL_GPTIM_CLOCKDIVISION(init->clockDivision));
    
    /* ???????????????? */
    if(TIMx == GPTIM0)
    {
        FL_RCC_EnableGroup4BusClock(FL_RCC_GROUP4_BUSCLK_GPTIM0);
    }
    else if (TIMx == GPTIM1)
    {
        FL_RCC_EnableGroup4BusClock(FL_RCC_GROUP4_BUSCLK_GPTIM1);
    }
    
    /* ?????????????????? */
    switch(init->counterMode)
    {
        /* ???????????? */
        case FL_GPTIM_COUNTER_ALIGNED_CENTER_DOWN   :   
        case FL_GPTIM_COUNTER_ALIGNED_CENTER_UP     : 
        case FL_GPTIM_COUNTER_ALIGNED_CENTER_UP_DOWN:
        FL_GPTIM_SetCounterAlignedMode(TIMx,init->counterMode);
        break;
        
        default:
        /* ???????? */
        FL_GPTIM_SetCounterDirection(TIMx,init->counterMode);
        FL_GPTIM_SetCounterAlignedMode(TIMx,FL_GPTIM_COUNTER_ALIGNED_EDGE);
        break;
    }
    
    /* ???????????? */
    FL_GPTIM_WriteAutoReload(TIMx,init->autoReload);
    
    /* ???????????????????????????????????????????????? */
    FL_GPTIM_SetClockDivision(TIMx,init->clockDivision);
    
    /* ???????? */
    FL_GPTIM_WritePrescaler(TIMx,init->prescaler);
    
    /* ?????????? */
    if(init->autoReloadState == ENABLE)
    {
        FL_GPTIM_EnableARRPreload(TIMx);
    }
    else
    {
        FL_GPTIM_DisableARRPreload(TIMx);
    }
    
    /* ?????????????????????????????? */
    FL_GPTIM_GenerateUpdateEvent(TIMx);
    
    return PASS;
}

/**
  * @brief	???? FL_GPTIM_InitTypeDef ??????????
  * @param 	TIM_InitStruct ?????????????????????????????????? @ref FL_GPTIM_InitTypeDef ??????  
  *         
  * @retval	None
  */
void FL_GPTIM_StructInit(FL_GPTIM_InitTypeDef *TIM_InitStruct)
{
    /* Set the default configuration */
    TIM_InitStruct->prescaler         = (uint16_t)0x0000;
    TIM_InitStruct->autoReloadState   = DISABLE;
    TIM_InitStruct->counterMode       = FL_GPTIM_COUNTER_DIR_UP;
    TIM_InitStruct->autoReload        = 0xFFFFU;
    TIM_InitStruct->clockDivision     = FL_GPTIM_CLK_DIVISION_DIV1;
}

/**
  * @brief  ????????????????????????????????????????????.
  * @param  TIMx Timer Instance
  * @param  slave_init ???????? @ref FL_GPTIM_SlaveInitTypeDef ?????? 
  * @retval ErrorStatus??????	
  *			-FAIL ????????????????        
  *			-PASS ????  
  */
ErrorStatus FL_GPTIM_SlaveMode_Init(GPTIM_Type *TIMx, FL_GPTIM_SlaveInitTypeDef *slave_init)
{
    /* ???????? */
    assert_param(IS_GPTIM_INSTANCE(TIMx)); 
    assert_param(IS_FL_GPTIM_SLAVE_MODE(slave_init->slaveMode));
    assert_param(IS_FL_GPTIM_TRIGGER_SRC(slave_init->triggerSrc));
    assert_param(IS_FL_GPTIM_TRIGGER_DELAY(slave_init->triggerDelay));
    
    /* ???????????????? */
    if(TIMx == GPTIM0)
    {
        FL_RCC_EnableGroup4BusClock(FL_RCC_GROUP4_BUSCLK_GPTIM0);
    }
    else if (TIMx == GPTIM1)
    {
        FL_RCC_EnableGroup4BusClock(FL_RCC_GROUP4_BUSCLK_GPTIM1);
    }
    
    /* ???????????????? */
    FL_GPTIM_DisableMasterSlaveMode(TIMx);
    
    /* ??????????????????TS */  
    FL_GPTIM_SetSlaveMode(TIMx,0);
    
    /* ???????????????? */
    FL_GPTIM_SetTriggerInput(TIMx,slave_init->triggerSrc);
    
    /* ITRx ?????????? */
    if(slave_init->triggerSrc <= FL_GPTIM_TIM_TS_ITR3)
    {
        /* ????????ITRx?????? */
        FL_GPTIM_SetITRInput(TIMx, (1U << (slave_init->triggerSrc >> GPTIM_SMCR_TS_Pos)), slave_init->ITRSourceGroup);
    }
    
    /* ?????????? */
    FL_GPTIM_SetSlaveMode(TIMx,slave_init->slaveMode);
    
    /* ???????????????? */
    if(slave_init->triggerDelay == ENABLE)
    {
        FL_GPTIM_EnableMasterSlaveMode(TIMx);
    }
    
    /* ?????????????????????????????? */
    FL_GPTIM_GenerateUpdateEvent(TIMx);
    
    return PASS;
}

/**
  * @brief	???? FL_GPTIM_SlaveInitTypeDef ??????????
  * @param 	TIM_InitStruct ?????????????????????????????????? @ref FL_GPTIM_SlaveInitTypeDef ??????  
  *         
  * @retval	None
  */
void FL_GPTIM_SlaveMode_StructInit(FL_GPTIM_SlaveInitTypeDef *slave_init)
{
    slave_init->ITRSourceGroup  = 0;
    slave_init->slaveMode       = FL_GPTIM_SLAVE_MODE_PROHIBITED;     
    slave_init->triggerSrc      = FL_GPTIM_TIM_TS_TI1FP1;
    slave_init->triggerDelay    = DISABLE;

}
/**
  * @brief  ????TIM????????????????ETR.
  * @param  TIMx Timer Instance
  * @param  ETPolarity   ????
  * @param  ETPrescaler  ????
  * @param  ETR_Filter   ????
  * @param  etr_init ???????? @ref FL_GPTIM_ETR_InitTypeDef ??????
  * @retval ErrorStatus??????
  *			-FAIL ????????????????        
  *			-PASS ????  
  */
ErrorStatus  FL_GPTIM_ETR_Init(GPTIM_Type *TIMx, FL_GPTIM_ETR_InitTypeDef* etr_init)
{
    assert_param(IS_FL_GPTIM_ETR_FILTER(etr_init->ETRFilter));
    assert_param(IS_FL_GPTIM_ETR_PSC(etr_init->ETRClockDivision));                                                      
    assert_param(IS_FL_GPTIM_ETR_POLARITY(etr_init->ETRPolarity));

    /* ???????????????? */
    if(TIMx == GPTIM0)
    {
        FL_RCC_EnableGroup4BusClock(FL_RCC_GROUP4_BUSCLK_GPTIM0);
    }
    else if (TIMx == GPTIM1)
    {
        FL_RCC_EnableGroup4BusClock(FL_RCC_GROUP4_BUSCLK_GPTIM1);
    }

    /* ???????????? */
    FL_GPTIM_SetETRPolarity(TIMx, etr_init->ETRPolarity);
    
    /* ???????????? */
    FL_GPTIM_SetETRFilter(TIMx, etr_init->ETRFilter);
    
    /* ???????????? */
    FL_GPTIM_SetETRPrescaler(TIMx, etr_init->ETRClockDivision);
    
    if(etr_init->useExternalTrigger == ENABLE)
    {
        FL_GPTIM_EnableExternalClock(TIMx);
    }
    else
    {
        FL_GPTIM_DisableExternalClock(TIMx);
    }
    
    /* ?????????????????????????????? */
    FL_GPTIM_GenerateUpdateEvent(TIMx);
    
    return PASS;
}

/**
  * @brief	???? FL_GPTIM_ETRInitTypeDef ??????????
  * @param 	etr_init ?????????????????????????????????? @ref FL_GPTIM_ETR_InitTypeDef ??????  
  *         
  * @retval	None
  */
void FL_GPTIM_ETR_StructInit(FL_GPTIM_ETR_InitTypeDef *etr_init)
{
    etr_init->useExternalTrigger  = DISABLE;
    etr_init->ETRFilter           = FL_GPTIM_ETR_FILTER_DIV1;       
    etr_init->ETRPolarity         = FL_GPTIM_ETR_POLARITY_NORMAL;     
    etr_init->ETRClockDivision    = FL_GPTIM_ETR_PSC_DIV1;
}

/**
  * @brief  ????TIM??????????????.
  * @param  TIMx Timer Instance
  * @param  Channel This parameter can be one of the following values:
  *         @arg @ref FL_GPTIM_CHANNEL_0
  *         @arg @ref FL_GPTIM_CHANNEL_1
  *         @arg @ref FL_GPTIM_CHANNEL_2
  *         @arg @ref FL_GPTIM_CHANNEL_3
  * @param  oc_init ???????? @ref FL_GPTIM_OC_InitTypeDef ??????
  * @retval ErrorStatus??????
  *			-FAIL ????????????????        
  *			-PASS ????  
  */
ErrorStatus FL_GPTIM_OC_Init(GPTIM_Type *TIMx, uint32_t channel, FL_GPTIM_OC_InitTypeDef *oc_init)
{
    ErrorStatus result = PASS;
    
    /* ???????? */
    assert_param(IS_GPTIM_INSTANCE(TIMx)); 
    assert_param(IS_FL_GPTIM_OC_MODE(oc_init->OCMode));
    assert_param(IS_FL_GPTIM_OC_PRELOAD(oc_init->OCPreload));
    assert_param(IS_FL_GPTIM_OC_POLARITY(oc_init->OCPolarity));
    assert_param(IS_FL_GPTIM_OC_FASTMODE(oc_init->OCFastMode));
    assert_param(IS_FL_GPTIM_OC_ETR_CLEARN(oc_init->OCETRFStatus));
    
    /* ???????? */
    FL_GPTIM_OC_DisableChannel(TIMx, channel);
    
    /* ???????? */
    FL_GPTIM_OC_SetChannelPolarity(TIMx,oc_init->OCPolarity,channel);
    
    /* ?????????????????? */
    FL_GPTIM_CC_SetChannelMode(TIMx, FL_GPTIM_CHANNEL_MODE_OUTPUT, channel);
    
    /* ?????????????????????? */
    OCConfig(TIMx,channel,oc_init);
    
    /* ???????? */
    FL_GPTIM_OC_EnableChannel(TIMx, channel);
    
    /* ?????????????????????????????? */
    FL_GPTIM_GenerateUpdateEvent(TIMx);
    
    return result;
}
/**
  * @brief	????
  * @param  TIMx Timer Instance
  * @param  Channel This parameter can be one of the following values:
  *         @arg @ref FL_GPTIM_CHANNEL_0
  *         @arg @ref FL_GPTIM_CHANNEL_1
  *         @arg @ref FL_GPTIM_CHANNEL_2
  *         @arg @ref FL_GPTIM_CHANNEL_3 
  * @param 	oc_init ?????????????????????????????????? @ref FL_GPTIM_OC_InitTypeDef ??????????????????.
  * @retval	ErrorStatus??????
  *			-FAIL ????????????????        
  *			-PASS ????  
  */
static ErrorStatus OCConfig(GPTIM_Type *TIMx, uint32_t channel, FL_GPTIM_OC_InitTypeDef *oc_init)
{
    ErrorStatus result = PASS;
    
    /* ???????????????????? */
    FL_GPTIM_OC_SetMode(TIMx, oc_init->OCMode,channel);
    
    /* ????ETRF???????? */
    if(oc_init->OCETRFStatus == ENABLE)
    {
        FL_GPTIM_OC_EnableClear(TIMx,channel);
    }
    /* ???????????????????? */
    if(oc_init->OCFastMode == ENABLE )
    {
        FL_GPTIM_OC_EnableFastMode(TIMx,channel);
    }
    /* ???????????????????? */
    if(oc_init->OCPreload == ENABLE)
    {
        FL_GPTIM_OC_EnablePreload(TIMx,channel);
    }
    /* ?????????? */
    switch(channel)
    {
        case FL_GPTIM_CHANNEL_1:
            FL_GPTIM_WriteCompareCH1(TIMx,oc_init->compareValue);
            break;
        case FL_GPTIM_CHANNEL_2:
            FL_GPTIM_WriteCompareCH2(TIMx,oc_init->compareValue);
            break;
        case FL_GPTIM_CHANNEL_3:
            FL_GPTIM_WriteCompareCH3(TIMx,oc_init->compareValue);
            break;
        case FL_GPTIM_CHANNEL_4:
            FL_GPTIM_WriteCompareCH4(TIMx,oc_init->compareValue);
            break;
        default :
            result = FAIL;
            break;

    }
    return result;
}
/**
  * @brief	???? FL_GPTIM_OC_InitTypeDef ??????????
  * @param 	oc_init ?????????????????????????????????? @ref FL_GPTIM_OC_InitTypeDef ??????  
  *         
  * @retval	None
  */
void FL_GPTIM_OC_StructInit(FL_GPTIM_OC_InitTypeDef *oc_init)
{
  /* Set the default configuration */
  oc_init->OCMode       = FL_GPTIM_OC_MODE_FROZEN;
  oc_init->OCETRFStatus = DISABLE;
  oc_init->OCFastMode   = DISABLE;
  oc_init->compareValue = 0x00000000U;
  oc_init->OCPolarity   = FL_GPTIM_OC_POLARITY_NORMAL;
  oc_init->OCPreload    = DISABLE;

}
/**
  * @brief  ????TIM??????????????.
  * @param  TIMx Timer Instance
  * @param  Channel This parameter can be one of the following values:
  *         @arg @ref FL_GPTIM_CHANNEL_0
  *         @arg @ref FL_GPTIM_CHANNEL_1
  *         @arg @ref FL_GPTIM_CHANNEL_2
  *         @arg @ref FL_GPTIM_CHANNEL_3
  * @param  ic_init ???????? @ref FL_GPTIM_IC_InitTypeDef ??????
  * @retval ErrorStatus??????
  *			-FAIL ????????????????        
  *			-PASS ????  
  */
ErrorStatus FL_GPTIM_IC_Init(GPTIM_Type *TIMx, uint32_t channel, FL_GPTIM_IC_InitTypeDef* ic_init)
{
    ErrorStatus result = PASS;
    
    /* ???????? */
    assert_param(IS_FL_GPTIM_CHANNEL(channel));
    assert_param(IS_FL_GPTIM_IC_CAPTURE_STATE(ic_init->captureState));
    assert_param(IS_FL_GPTIM_IC_POLARITY(ic_init->ICPolarity));
    assert_param(IS_FL_GPTIM_CHANNEL_MODE(ic_init->ICActiveInput));                             
    assert_param(IS_FL_GPTIM_IC_PSC(ic_init->ICPrescaler));
    assert_param(IS_FL_GPTIM_IC_FILTER(ic_init->ICFilter));
    
    /* ???????????????? */
    if(TIMx == GPTIM0)
    {
        FL_RCC_EnableGroup4BusClock(FL_RCC_GROUP4_BUSCLK_GPTIM0);
    }
    else if (TIMx == GPTIM1)
    {
        FL_RCC_EnableGroup4BusClock(FL_RCC_GROUP4_BUSCLK_GPTIM1);
    }
    
    /* ???????? */
    FL_GPTIM_IC_DisableChannel(TIMx,channel);
    
    /*???????? */
    FL_GPTIM_IC_SetChannelPolarity(TIMx,ic_init->ICPolarity,channel);
    
    /* ???????????? */
    FL_GPTIM_CC_SetChannelMode(TIMx,ic_init->ICActiveInput,channel);
    
    /* ?????????? */
    FL_GPTIM_IC_SetPrescaler(TIMx,ic_init->ICPrescaler,channel);
    
    /* ?????????? */
    FL_GPTIM_IC_SetFilter(TIMx,ic_init->ICFilter,channel);
    
    if(ic_init->captureState == ENABLE)
    {
        FL_GPTIM_IC_EnableChannel(TIMx,channel);
    }
    /* ?????????????????????????????? */
    FL_GPTIM_GenerateUpdateEvent(TIMx);
    
    return result;
}

/**
  * @brief	???? FL_GPTIM_IC_InitTypeDef ??????????
  * @param 	ic_init ?????????????????????????????????? @ref FL_GPTIM_IC_InitTypeDef ??????  
  *         
  * @retval	None
  */
void FL_GPTIM_IC_StructInit(FL_GPTIM_IC_InitTypeDef *ic_init)
{
  /* Set the default configuration */  
  ic_init->ICPolarity    = FL_GPTIM_IC_POLARITY_NORMAL;
  ic_init->ICActiveInput = FL_GPTIM_CHANNEL_MODE_INPUT_NORMAL;
  ic_init->ICPrescaler   = FL_GPTIM_IC_PSC_DIV1;
  ic_init->ICFilter      = FL_GPTIM_IC_FILTER_DIV1;
  ic_init->captureState  = DISABLE;
}
/**
  * @}
  */
/**
* @}
*/
/**************************Py_Code_Generator Version: 0.1-0.5-0.1 @ 2019-10-21**************************/
/*************************(C) COPYRIGHT Fudan Microelectronics **** END OF FILE*************************/
