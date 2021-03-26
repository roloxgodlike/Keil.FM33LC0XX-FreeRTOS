/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : mf_config.c
  * @brief          : MCU FUNCTION CONFIG
  ******************************************************************************
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
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "mf_config.h"

/* Private function prototypes -----------------------------------------------*/


/**
  * @brief  NVIC Initialization function
  * @param  void
  * @retval None
  */
void MF_NVIC_Init(void)
{


    NVIC_ConfigTypeDef    InterruptConfigStruct;

    InterruptConfigStruct.preemptPriority = 0x02;
    NVIC_Init(&InterruptConfigStruct,LPTIM_IRQn );

    
}


 
/**
  * @brief  ADC_Common Initialization function
  * @param  void
  * @retval None
  */
void MF_ADC_Common_Init(void)
{

    /*IO CONFIG*/
    FL_ADC_CommonInitTypeDef    CommonInitStruct;

    CommonInitStruct.clockSource = FL_RCC_ADC_CLK_SOURCE_RCHF;
    CommonInitStruct.clockPrescaler = FL_RCC_ADC_PSC_DIV8;

    FL_ADC_CommonInit(&CommonInitStruct );

    
}
 
/**
  * @brief  ADC Initialization function
  * @param  void
  * @retval None
  */
void MF_ADC_Init(void)
{

    FL_GPIO_InitTypeDef    GPIO_InitStruct;

    FL_ADC_InitTypeDef    defaultInitStruct;

    defaultInitStruct.conversionMode = FL_ADC_CONV_MODE_SINGLE;
    defaultInitStruct.autoMode = FL_ADC_SINGLE_CONV_MODE_AUTO;
    defaultInitStruct.waitMode = ENABLE;
    defaultInitStruct.overrunMode = ENABLE;
    defaultInitStruct.scanDirection = FL_ADC_SEQ_SCAN_DIR_BACKWARD;
    defaultInitStruct.externalTrigConv = FL_ADC_TRIGGER_EDGE_NONE;
    defaultInitStruct.triggerSource = FL_ADC_TRGI_PA8;
    defaultInitStruct.fastChannelTime = FL_ADC_FAST_CH_SAMPLING_TIME_4_ADCCLK;
    defaultInitStruct.lowChannelTime = FL_ADC_SLOW_CH_SAMPLING_TIME_192_ADCCLK;
    defaultInitStruct.oversamplingMode = ENABLE;
    defaultInitStruct.overSampingMultiplier = FL_ADC_OVERSAMPLING_MUL_16X;
    defaultInitStruct.oversamplingShift = FL_ADC_OVERSAMPLING_SHIFT_4B;

    FL_ADC_Init(ADC,&defaultInitStruct );

    GPIO_InitStruct.pin = FL_GPIO_PIN_9;
    GPIO_InitStruct.mode = FL_GPIO_MODE_ANALOG;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = DISABLE;
    GPIO_InitStruct.remapPin = DISABLE;

    FL_GPIO_Init( GPIOC, &GPIO_InitStruct );

    
}


/**
  * @brief  BSTIM32 Initialization function
  * @param  void
  * @retval None
  */
void MF_BSTIM32_Init(void)
{

    /*IO CONFIG*/
    FL_BSTIM32_InitTypeDef    defaultInitStruct;

    defaultInitStruct.prescaler = 8000;
    defaultInitStruct.autoReload = 1000;
    defaultInitStruct.autoReloadState = ENABLE;
    defaultInitStruct.clockSource = FL_RCC_BSTIM32_CLK_SOURCE_APB1CLK;

    FL_BSTIM32_Init(BSTIM32,&defaultInitStruct );


    FL_BSTIM32_DisableIT_Update(BSTIM32 );

    
}


/**
  * @brief  LPTIM32 Initialization function
  * @param  void
  * @retval None
  */
void MF_LPTIM32_Init(void)
{

    /*IO CONFIG*/
    FL_LPTIM32_InitTypeDef    defaultInitStruct;

    defaultInitStruct.clockSource = FL_RCC_LPTIM32_CLK_SOURCE_APB1CLK;
    defaultInitStruct.prescalerClockSource = FL_LPTIM32_CLK_SOURCE_INTERNAL;
    defaultInitStruct.prescaler = FL_LPTIM32_PSC_DIV8;
    defaultInitStruct.autoReload = 1000;
    defaultInitStruct.mode = FL_LPTIM32_OPERATION_MODE_NORMAL;
    defaultInitStruct.onePulseMode = FL_LPTIM32_ONE_PULSE_MODE_CONTINUOUS;
    defaultInitStruct.countEdge = FL_LPTIM32_ETR_COUNT_EDGE_RISING;
    defaultInitStruct.triggerEdge = FL_LPTIM32_ETR_TRIGGER_EDGE_RISING;

    FL_LPTIM32_Init(LPTIM32,&defaultInitStruct );


    FL_LPTIM32_EnableIT_Update(LPTIM32 );

    
}

/**
  * @brief  The application entry point.
  * @retval int
  */
void MF_Clock_Init(void)
{
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */

  /* System interrupt init*/

  /* Initialize all configured peripherals */
 
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void MF_SystemClock_Config(void)
{
 
}

void MF_Config_Init(void)
{
    /*FUNCTION CALL*/
    /*ADC*/
     MF_ADC_Common_Init();
     MF_ADC_Init();
    /*BSTIM32*/
     MF_BSTIM32_Init();
    /*LPTIM32*/
     MF_LPTIM32_Init();
    /*使能外设中断*/
     MF_NVIC_Init();
    
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
