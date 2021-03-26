#include "main.h"
#include "user_init.h"
#include "FreeRTOS.h"
#include "TaskSystem.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "gpio.h"
#include "uart.h"
#include "pll.h"
/**
  ****************************************************************************************************
  * @file    main.c
  * @author  FMSH Application Team
  * @brief   Header file of  LL Module
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

#define TASK_LED_PRIORITY       (tskIDLE_PRIORITY + 1)
#define TASK_ADC_PRIORITY      (tskIDLE_PRIORITY + 1)
#define TASK_UART_PRIORITY      (tskIDLE_PRIORITY + 1)

uint8_t TestRxData[100];

QueueHandle_t ADC_QueueHandle;   
QueueHandle_t dmaSemaphore;

void HardFault_Handler(void)
{
	printf("hardfault\n");
}

void Led0Task(void *pvParameters)
{
    pvParameters = pvParameters;
    while (1)
    {
        delay_ms(50);
        LED0_TOG;
    }
}

void Led1Task(void *pvParameters)
{
    pvParameters = pvParameters;
    while (1)
    {
        delay_ms(100);
        LED1_TOG;
    }
}

void Led2Task(void *pvParameters)
{
    pvParameters = pvParameters;
    while (1)
    {
        delay_ms(300);
//        vTaskSuspendAll();
        LED2_ON;
        delay_ms(3000);
//        delay_xms(3000);
        LED2_OFF;
//        if (!xTaskResumeAll())
//        {
//            taskYIELD();
//        }
    }
}

int main(void)
{
    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    
    /* SHOULD BE KEPT!!! */
    MF_Clock_Init();
    
    /* Configure the system clock */
    /* SHOULD BE KEPT!!! */
    MF_SystemClock_Config();
    
    /* user init */
    UserInit();
    SelXTHFToPLL(FL_RCC_PLL_PSC_DIV8, FL_RCC_PLL_MUL_24);
    DebugUartInit(115200);
    LedInit();
    
    /* Initialize all configured peripherals */
    /* SHOULD BE KEPT!!! */
    /* MF 初始化*/
    MF_Config_Init();
    
    printf("RCC->PLLCR: %u\n", RCC->PLLCR);
    printf("RCC->SYSCLKCR: %u\n", RCC->SYSCLKCR);
    printf("SYSCLK freq: %u\n", FL_RCC_GetSystemClockFreq());
    delay_init();

    /* UART5 DMA初始化 */
    UARTx_DMA(UART5, TestRxData, 10); 
    /* GPIO 初始化 */
    GPIO_Init();
    
    ADC_QueueHandle = xSemaphoreCreateCounting(10, 0);
    dmaSemaphore = xSemaphoreCreateCounting(10, 0);
    xTaskCreate(Led0Task, "Led0Task", configMINIMAL_STACK_SIZE, NULL, TASK_LED_PRIORITY, NULL);      
    xTaskCreate(Led1Task, "Led1Task", configMINIMAL_STACK_SIZE, NULL, TASK_LED_PRIORITY, NULL);      
    xTaskCreate(Led2Task, "Led2Task", configMINIMAL_STACK_SIZE, NULL, TASK_LED_PRIORITY, NULL);      
//    xTaskCreate(AdcTask, "Adc", configMINIMAL_STACK_SIZE, NULL, TASK_ADC_PRIORITY, NULL);    
    xTaskCreate(UartTask, "Uart", configMINIMAL_STACK_SIZE, NULL, TASK_UART_PRIORITY, NULL);       
    
    vTaskStartScheduler();
    
    NVIC_SystemReset();//重启
    
    while(1)
    {     
    }
}

void vApplicationIdleHook(void)
{
    // TODO: 空闲任务
}
