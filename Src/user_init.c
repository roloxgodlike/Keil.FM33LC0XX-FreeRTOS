#include "user_init.h"
#include "gpio.h"

#include "FreeRTOS.h"
#include "task.h"


void ClockInit(uint32_t clock)
{
    switch (clock)
    {
        case FL_RCC_RCHF_FREQUENCY_8MHZ:
            FL_RCC_RCHF_WriteTrimValue(RCHF8M_TRIM);
            break;
        
        case FL_RCC_RCHF_FREQUENCY_16MHZ:
            FL_RCC_RCHF_WriteTrimValue(RCHF16M_TRIM);
            break;
        
        case FL_RCC_RCHF_FREQUENCY_24MHZ:
            FL_RCC_RCHF_WriteTrimValue(RCHF24M_TRIM);
            break;
        
        default:
            FL_RCC_RCHF_WriteTrimValue(RCHF8M_TRIM);
            break;
    }
    
    FL_RCC_RCHF_SetFrequency(clock);
    FL_RCC_SetSystemClockSource(FL_RCC_SYSTEM_CLK_SOURCE_RCHF);
}

void SystickInit(void)
{
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

void FoutInit(void)
{
    FL_GPIO_InitTypeDef init = {0};
    
    init.pin = FL_GPIO_PIN_11;
    init.mode = FL_GPIO_MODE_DIGITAL;
    init.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    init.pull = DISABLE;
    FL_GPIO_Init(GPIOD, &init);
    
    FL_GPIO_SetFOUT0(GPIO, FL_GPIO_FOUT0_SELECT_AHBCLK_DIV64);
}

#ifndef MFANG
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
    int handle; 
}; 
FILE __stdout;    

//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
    x = x; 
} 

//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
    FL_UART_WriteTXBuff(UART0, (uint8_t)ch);
    while(FL_UART_IsActiveFlag_TXBuffEmpty(UART0) != SET);
    return ch;
}

void DebugUartInit(uint32_t baud)
{
    FL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    FL_UART_InitTypeDef UART_InitStruct = {0};   

   FL_UART_DeInit(UART0);
    
    //PA13:UART0-RX   PA14:UART0-TX
    GPIO_InitStruct.pin        = FL_GPIO_PIN_13|FL_GPIO_PIN_14;
    GPIO_InitStruct.mode       = FL_GPIO_MODE_DIGITAL;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;   
    GPIO_InitStruct.pull       = ENABLE;
    GPIO_InitStruct.remapPin = DISABLE;
    FL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    UART_InitStruct.clockSrc = FL_RCC_UART0_CLK_SOURCE_APB1CLK;
	
	UART_InitStruct.baudRate            = baud;							        //波特率
	UART_InitStruct.dataWidth           = FL_UART_DATA_WIDTH_8B;			      	//数据位数
	UART_InitStruct.stopBits            = FL_UART_STOP_BIT_WIDTH_1B;				//停止位
	UART_InitStruct.parity              = FL_UART_PARITY_NONE;				        //奇偶校验
	UART_InitStruct.transferDirection   = FL_UART_DIRECTION_TX;	                //接收-发送使能
	FL_UART_Init(UART0, &UART_InitStruct);	
}

void LedInit(void)
{
    FL_GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    GPIO_InitStruct.pin = LED0_PIN;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = DISABLE;
    FL_GPIO_Init(LED0_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.pin = LED1_PIN;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = DISABLE;
    FL_GPIO_Init(LED1_GPIO, &GPIO_InitStruct);
    
    GPIO_InitStruct.pin = LED2_PIN;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = DISABLE;
    FL_GPIO_Init(LED2_GPIO, &GPIO_InitStruct);
    
    LED0_ON;
    LED1_ON;
    LED2_ON;
}

#endif 

void UserInit(void)
{
    SystickInit();
    DebugUartInit(115200);
}

//uint8_t fac_us;
static uint32_t fac_us=0;							//us延时倍乘数
static uint16_t fac_ms=0;				        //ms延时倍乘数,在os下,代表每个节拍的ms数

extern void xPortSysTickHandler(void);
//systick中断服务函数,使用OS时用到
void SysTick_Handler(void)
{  
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)//系统已经运行
    {
        xPortSysTickHandler();	
    }
}
			   
//SYSCLK:系统时钟频率
void delay_init(void)
{
    memset(SysTick, 0, sizeof(SysTick_Type));
    uint8_t freq = FL_RCC_GetSystemClockFreq() / 1000000;
	uint32_t reload;
	fac_us = freq;						    //不论是否使用OS,fac_us都需要使用
	reload = freq;					        //每秒钟的计数次数 单位为K	   
	reload *= 1000000 / configTICK_RATE_HZ;		//根据configTICK_RATE_HZ设定溢出时间
											//reload为24位寄存器,最大值:16777216,在180M下,约合0.745s左右	
	fac_ms = 1000 / configTICK_RATE_HZ;			//代表OS可以延时的最少单位
    
    NVIC_SetPriority(SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL); /* set Priority for Systick Interrupt */
	SysTick->LOAD = reload; 					//每1/configTICK_RATE_HZ断一次	
    SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
               SysTick_CTRL_TICKINT_Msk   |
               SysTick_CTRL_ENABLE_Msk;                         /* Enable SysTick IRQ and SysTick Timer */
}								    

//延时nus
//nus:要延时的us数.	
//nus:0~190887435(最大值即2^32/fac_us@fac_us=22.5)	    								   
void delay_us(uint32_t nus)
{		
	uint32_t ticks;
	uint32_t told, tnow, tcnt = 0;
	uint32_t reload = SysTick->LOAD;				//LOAD的值	    	 
	ticks = nus * fac_us; 						//需要的节拍数 
	told = SysTick->VAL;        				//刚进入时的计数器值
	while (1)
	{
		tnow = SysTick->VAL;	
		if (tnow != told)
		{	    
			if (tnow < told) tcnt += told - tnow;	//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt += reload - tnow + told;	    
			told = tnow;
			if (tcnt >= ticks) break;			//时间超过/等于要延迟的时间,则退出.
		}  
	};									    
}  
	
//延时nms,会引起任务调度
//nms:要延时的ms数
//nms:0~65535
void delay_ms(uint32_t nms)
{	
	if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)//系统已经运行
	{		
		if (nms >= fac_ms)						//延时的时间大于OS的最少时间周期 
		{ 
   			vTaskDelay(nms / fac_ms);	 		//FreeRTOS延时
		}
		nms %= fac_ms;						//OS已经无法提供这么小的延时了,采用普通方式延时    
	}
	delay_us((uint32_t)(nms * 1000));				//普通方式延时
}

//延时nms,不会引起任务调度
//nms:要延时的ms数
void delay_xms(uint32_t nms)
{
	uint32_t i;
	for (i = 0; i < nms; i++) delay_us(1000);
}
