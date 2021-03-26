/**
  ****************************************************************************************************
  * @file    fm33lC0xx_fl_rcc.c
  * @author  FMSH Application Team
  * @brief   Src file of RCC FL Module
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
#include "system_fm33lc0xx.h"
#include "fm33lc0xx_fl_rcc.h"
#include "fm33_assert.h"

/** @addtogroup FM33LC0xx_FL_Driver_RCC
  * @{
  */

/** @addtogroup RCC_FL_OPEREAT
  * @{
  */
  
/**
  * @brief  获取 USB提供给系统总线时钟的频率。
  *
  * @param  None 
  *         
  * @retval 返回USB提供给SYSCLK的时钟频率            
  */
uint32_t FL_RCC_GetUSBClockFreqToSysclk(void)
{
    if(FL_RCC_GetUSBClockSource() == FL_RCC_USB_CLOCK_SELECT_48M)
    {
        return 48000000;
    }
    else 
    {
        return 60000000;
    }
}

/**
  * @brief  获取系统当前工作时钟SYSCLK。
  * @param  None
  * @note   函数中用到了XTHF_VALUE 宏，这个宏应该被定义为外部晶振的输入频率值。
  *
  * @retval 系统时钟频率 (Hz)。
  *                
  */
uint32_t FL_RCC_GetSystemClockFreq(void)
{
    uint32_t frequency;

    /* 获取系统时钟源 */
    switch (FL_RCC_GetSystemClockSource()) 
    {
        /* 系统时钟源为内部RCHF */
        case FL_RCC_SYSTEM_CLK_SOURCE_RCHF:
            /* 内部RCHF默认为8MHz ,可以配置为16或24M */
            frequency = FL_RCC_GetRCHFClockFreq();
            break;
            /* 系统时钟源为XTHF */
        case FL_RCC_SYSTEM_CLK_SOURCE_XTHF:
            frequency = XTHF_VALUE;
            break;
            /* 系统时钟源为PLL */
        case FL_RCC_SYSTEM_CLK_SOURCE_PLL:
            frequency = FL_RCC_GetPLLClockFreq();
            break;
            /* 系统时钟源为内部RCMF */
        case FL_RCC_SYSTEM_CLK_SOURCE_RCMFPSC:
            /* 根据RC4M的分频配置得出系统时钟 */
            frequency = FL_RCC_GetRC4MClockFreq();
            break;
            /* 系统时钟源为XTLF */
        case FL_RCC_SYSTEM_CLK_SOURCE_XTLF:
            /* 根据外部晶振的频率得出系统时钟 */
            frequency = XTLF_VALUE;
            break;
        case FL_RCC_SYSTEM_CLK_SOURCE_USBCLK:
            /* USB时钟频率获取*/
            frequency = FL_RCC_GetUSBClockFreqToSysclk();
            break;
            /* 系统时钟源为RCLP */
        case FL_RCC_SYSTEM_CLK_SOURCE_RCLP:
            frequency = 32000;
            break;
    }
    return frequency;
}
/**
  * @brief  获取 AHB 总线时钟频率。
  *
  * @param  SYSCLK_Frequency  系统主时钟频率SYSCLK
  *         
  * @retval AHB 总线时钟频率(Hz)
  *                 
  */
uint32_t FL_RCC_GetAHBClockFreq(void)
{
    uint32_t frequency;
    /* 获取AHB分频系数，AHB源自系统主时钟 */
    switch(FL_RCC_GetAHBPrescaler())
    {
        case FL_RCC_AHBCLK_PSC_DIV1:
            frequency = FL_RCC_GetSystemClockFreq();
            break;
        case FL_RCC_AHBCLK_PSC_DIV2:
            frequency = FL_RCC_GetSystemClockFreq()/2;
            break;
        case FL_RCC_AHBCLK_PSC_DIV4:
            frequency = FL_RCC_GetSystemClockFreq()/4;
            break;  
        case FL_RCC_AHBCLK_PSC_DIV8:
            frequency = FL_RCC_GetSystemClockFreq()/8;
            break;            
        case FL_RCC_AHBCLK_PSC_DIV16:
            frequency = FL_RCC_GetSystemClockFreq()/16;
            break;
    }
    return frequency;
}

/**
  * @brief  获取当前系统的APB1总线时钟
  * @param  AHB1_Frequency AHB总线的时钟频率
  *         
  * @retval APB1 clock frequency (in Hz)
  *              
  */
uint32_t FL_RCC_GetAPB1ClockFreq(void)
{
    uint32_t frequency;
    /* 获取APB1分频系数，APB源自AHB */
    switch(FL_RCC_GetAPB1Prescaler())
    {
        case FL_RCC_APB1CLK_PSC_DIV1:
            frequency = FL_RCC_GetAHBClockFreq();
            break;
        case FL_RCC_APB1CLK_PSC_DIV2:
            frequency = FL_RCC_GetAHBClockFreq()/2;
            break;
        case FL_RCC_APB1CLK_PSC_DIV4:
            frequency = FL_RCC_GetAHBClockFreq()/4;
            break;  
        case FL_RCC_APB1CLK_PSC_DIV8:
            frequency = FL_RCC_GetAHBClockFreq()/8;
            break;            
        case FL_RCC_APB1CLK_PSC_DIV16:
            frequency = FL_RCC_GetAHBClockFreq()/16;
            break;
    }
    return frequency;
}
/**
  * @brief  获取当前系统的APB2总线时钟
  * @param  AHB2_Frequency AHB总线的时钟频率
  *         
  * @retval APB2 clock frequency (in Hz)
  *              
  */
uint32_t FL_RCC_GetAPB2ClockFreq(void)
{
    uint32_t frequency;
    /* 获取APB2分频系数，APB源自AHB */
    switch(FL_RCC_GetAPB2Prescaler())
    {
        case FL_RCC_APB2CLK_PSC_DIV1:
            frequency = FL_RCC_GetAHBClockFreq();
            break;
        case FL_RCC_APB2CLK_PSC_DIV2:
            frequency = FL_RCC_GetAHBClockFreq()/2;
            break;
        case FL_RCC_APB2CLK_PSC_DIV4:
            frequency = FL_RCC_GetAHBClockFreq()/4;
            break;  
        case FL_RCC_APB2CLK_PSC_DIV8:
            frequency = FL_RCC_GetAHBClockFreq()/8;
            break;            
        case FL_RCC_APB2CLK_PSC_DIV16:
            frequency = FL_RCC_GetAHBClockFreq()/16;
            break;
    }
    return frequency;
}
/**
  * @brief  获取RC4M输出时钟频率
  * @param  None
  *         
  * @retval 返回RC4M输出时钟频率(Hz)
  *                 
  */
uint32_t FL_RCC_GetRC4MClockFreq(void)
{
    uint32_t frequency;
    
    switch(FL_RCC_RCMF_GetPrescaler())
    {       
        case FL_RCC_RCMF_PSC_DIV1:
            frequency = 4000000;
            break;
        case FL_RCC_RCMF_PSC_DIV4:
            frequency = 1000000;
            break; 
        case FL_RCC_RCMF_PSC_DIV8:
            frequency = 500000;
            break;
        case FL_RCC_RCMF_PSC_DIV16:
            frequency = 250000;
            break;  
        default:frequency = 4000000;
            break;
    }
    return frequency;
}
/**
  * @brief  获取RCHF输出时钟频率
  * @param  None
  *         
  * @retval 返回RCHF输出时钟频率(Hz)
  *                 
  */
uint32_t FL_RCC_GetRCHFClockFreq(void)
{
    uint32_t frequency; 
    switch(FL_RCC_RCHF_GetFrequency())
    {
        case FL_RCC_RCHF_FREQUENCY_8MHZ:
            frequency = 8000000;
            break;
        case FL_RCC_RCHF_FREQUENCY_16MHZ:
            frequency = 16000000;
            break;
        case FL_RCC_RCHF_FREQUENCY_24MHZ:
            frequency = 24000000;
            break;
        default:
            frequency = 8000000;
            break;
    }
    return frequency;
}
/**
  * @brief  获取PLL输出时钟频率
  * @param  None
  *         
  * @retval 返回PLL输出时钟频率(Hz)
  *                 
  */
uint32_t FL_RCC_GetPLLClockFreq(void)
{
    uint32_t frequency,Multiplier;
    /* 获取PLL时钟源 */
    switch(FL_RCC_PLL_GetClockSource())
    {
        case FL_RCC_PLL_CLK_SOURCE_RCHF:
            /* 获取RCHF配置主频 */
            frequency = FL_RCC_GetRCHFClockFreq();
            break;
        case FL_RCC_PLL_CLK_SOURCE_XTHF:
            frequency = XTHF_VALUE;
            break;
    }
    /* 获取PLL时钟分频系数 */
    switch(FL_RCC_PLL_GetPrescaler())
    {
        case FL_RCC_PLL_PSC_DIV1:
            break;
        case FL_RCC_PLL_PSC_DIV2:
            frequency /= 2;
            break;
        case FL_RCC_PLL_PSC_DIV4:
            frequency /= 4;
            break;
        case FL_RCC_PLL_PSC_DIV8:
            frequency /= 8;
            break;
        case FL_RCC_PLL_PSC_DIV12:
            frequency /= 12;
            break;
        case FL_RCC_PLL_PSC_DIV16:
            frequency /= 16;
            break;
        case FL_RCC_PLL_PSC_DIV24:
            frequency /= 24;
            break;
        case FL_RCC_PLL_PSC_DIV32:
            frequency /= 32;
            break;     
    }
    Multiplier = (FL_RCC_PLL_ReadMultiplier()+1);
    frequency *= Multiplier;
          
    return frequency;
}

/**
  * @}
  */
/**
  * @}
  */

/*************************(C) COPYRIGHT Fudan Microelectronics **** END OF FILE*************************/

