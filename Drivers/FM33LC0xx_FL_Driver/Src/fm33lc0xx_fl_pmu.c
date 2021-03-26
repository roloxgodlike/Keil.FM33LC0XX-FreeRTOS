/**
  ****************************************************************************************************
  * @file    fm33lc0xx_ll_pmu.c
  * @author  FMSH Application Team
  * @brief   Src file of PMU LL Module
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
#include "fm33lc0xx_fl_rmu.h"
#include "fm33lc0xx_fl_pmu.h"
#include "fm33_assert.h"
/** @addtogroup FM33LC0xx_LL_Driver_PMU
  * @{
  */
/* Private macros ------------------------------------------------------------*/
/** @addtogroup UART_LL_Private_Macros
  * @{
  */



#define         IS_FL_PMU_INSTANCE(INSTANCE)                (((INSTANCE) == PMU))
                                                             
#define         IS_FL_PMU_MODE(__VALUE__)                   (((__VALUE__) == FL_PMU_POWER_MODE_ACTIVE_OR_LPACTIVE)||\
                                                             ((__VALUE__) == FL_PMU_POWER_MODE_LPRUN_ONLY)||\
                                                             ((__VALUE__) == FL_PMU_POWER_MODE_SLEEP_OR_DEEPSLEEP))
                                                             
#define         IS_FL_PMU_COREVOLTAGESCALING(__VALUE__)     (((__VALUE__) == DISABLE)||\
                                                             ((__VALUE__) == ENABLE))
                                                      
#define         IS_FL_PMU_DEEPSLEEP(__VALUE__)              (((__VALUE__) == FL_PMU_SLEEP_MODE_DEEP)||\
                                                             ((__VALUE__) == FL_PMU_SLEEP_MODE_NORMAL))
                                                             
#define         IS_FL_PMU_WAKEUPFREQUENCY(__VALUE__)        (((__VALUE__) == FL_PMU_RCHF_WAKEUP_FREQ_8MHZ)||\
                                                             ((__VALUE__) == FL_PMU_RCHF_WAKEUP_FREQ_16MHZ)||\
                                                             ((__VALUE__) == FL_PMU_RCHF_WAKEUP_FREQ_24MHZ))
                                                                         
#define         IS_FL_PMU_LDOLOWMODE(__VALUE__)             (((__VALUE__) == FL_PMU_LDO_LPM_DISABLE)||\
                                                             ((__VALUE__) == FL_PMU_LDO_LPM_ENABLE)) 
                                                             
#define         IS_FL_PMU_WAKEUPDELAY(__VALUE__)            (((__VALUE__) == FL_PMU_WAKEUP_DELAY_0US)||\
                                                             ((__VALUE__) == FL_PMU_WAKEUP_DELAY_2US)||\
                                                             ((__VALUE__) == FL_PMU_WAKEUP_DELAY_4US)||\
                                                             ((__VALUE__) == FL_PMU_WAKEUP_DELAY_8US))
                                                             
/** 
  *@} 
  */
/** @addtogroup PMU_LL_EF_Init
  * @{
  */   
/**
  * @brief	��λpmu����
  *
  * @param	������ڵ�ַ
  *
  * @retval	���ش���״̬������ֵ:
  *       	-PASS ����Ĵ���ֵ�ظ���λֵ
  *       	-FAIL λ�ɹ�ִ��
  */
ErrorStatus FL_PMU_Sleep_DeInit(PMU_Type* PMUx)
{
    ErrorStatus status = FAIL;
	/* �����Ϸ��Լ�� */
    assert_param(IS_FL_PMU_INSTANCE(PMUx));
    PMUx->CR   = 0x00060000U;
    PMUx->WKTR = 0xC0000001U;
    PMUx->IER  = 0x00000000U;
    status = PASS;
    return status;
}


/**
  * @brief	����lpm_initstruct�ṹ�������������Ϣ����pmu�Ĵ��� 
  *         
  * @note 	Ϊ���õ�˯�߹����û�������Ҫ����ʵ��Ӧ�ã����� @ref fm33lc0xx_ll_pmu.h�е������ӿ�
  *      	 �����˯��ǰ��ģʽ���ã�����˯����Ϊ�ͻ��Ѻ����Ϊ(ע���˺�����ر�BOR)      
  * @param	PMUx  ������ڵ�ַ
  * @param 	LPM_InitStruct ָ��һ�� @ref FL_PMU_SleepInitTypeDef ���͵Ľṹ�壬������ָ��LPUART�����������Ϣ	  
  *         
  * @retval	ErrorStatusö��ֵ	
  *			-FAIL ���ù��̷�������    
  *			-PASS LPUART���óɹ�      
  */
ErrorStatus FL_PMU_Sleep_Init(PMU_Type* PMUx, FL_PMU_SleepInitTypeDef *LPM_InitStruct)
{
    ErrorStatus status = FAIL;
	/* �����Ϸ��Լ�� */
    assert_param(IS_FL_PMU_INSTANCE(PMUx));
    assert_param(IS_FL_PMU_LDOLOWMODE(LPM_InitStruct->LDOLowPowerMode));
    assert_param(IS_FL_PMU_COREVOLTAGESCALING(LPM_InitStruct->coreVoltageScaling));
    assert_param(IS_FL_PMU_DEEPSLEEP(LPM_InitStruct->deepSleep));
    assert_param(IS_FL_PMU_WAKEUPFREQUENCY(LPM_InitStruct->wakeupFrequency));
    assert_param(IS_FL_PMU_WAKEUPDELAY(LPM_InitStruct->wakeupDelay));

	 /* ����ʱ�� */
    FL_PMU_SetWakeupDelay(PMUx,LPM_InitStruct->wakeupDelay);   
    /* ���Ѻ�RCHF��Ƶ�� */
    FL_PMU_SetRCHFWakeupFrequency(PMUx,LPM_InitStruct->wakeupFrequency);
    /* ˯�����ں˵�ѹ���� */
    if(LPM_InitStruct->coreVoltageScaling == ENABLE)
    {
        FL_PMU_EnableCoreVoltageScaling(PMUx);
    }
    else
    {
        FL_PMU_DisableCoreVoltageScaling(PMUx);
    }
		/* M0ϵͳ��������һ������Ϊ0����*/
		SCB->SCR =0;								
    /* ˯��ģʽ */
    FL_PMU_SetSleepMode(PMUx,LPM_InitStruct->deepSleep);

  	status = PASS;
    return status;
}

/**
  * @brief	LPM_InitStruct ΪĬ������
  * @param 	LPM_InitStruct ָ����Ҫ��ֵ����ΪĬ�����õĽṹ�� @ref FL_PMU_SleepInitTypeDef structure 
  *         �ṹ��
  * @retval	None
  */
void FL_PMU_StructInit(FL_PMU_SleepInitTypeDef *LPM_InitStruct)
{
    LPM_InitStruct->deepSleep           = FL_PMU_SLEEP_MODE_NORMAL;
    LPM_InitStruct->LDOLowPowerMode     = FL_PMU_LDO_LPM_DISABLE;
    LPM_InitStruct->wakeupFrequency     = FL_PMU_RCHF_WAKEUP_FREQ_8MHZ;
    LPM_InitStruct->wakeupDelay         = FL_PMU_WAKEUP_DELAY_2US;
    LPM_InitStruct->coreVoltageScaling  = DISABLE;
	
}
/** 
  *@} 
  */
/** 
  *@} 
  */
/*********************** (C) COPYRIGHT Fudan Microelectronics *****END OF FILE************************/
