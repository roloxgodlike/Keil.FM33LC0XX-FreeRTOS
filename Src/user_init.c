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
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
    int handle; 
}; 
FILE __stdout;    

//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
    x = x; 
} 

//�ض���fputc���� 
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
	
	UART_InitStruct.baudRate            = baud;							        //������
	UART_InitStruct.dataWidth           = FL_UART_DATA_WIDTH_8B;			      	//����λ��
	UART_InitStruct.stopBits            = FL_UART_STOP_BIT_WIDTH_1B;				//ֹͣλ
	UART_InitStruct.parity              = FL_UART_PARITY_NONE;				        //��żУ��
	UART_InitStruct.transferDirection   = FL_UART_DIRECTION_TX;	                //����-����ʹ��
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
static uint32_t fac_us=0;							//us��ʱ������
static uint16_t fac_ms=0;				        //ms��ʱ������,��os��,����ÿ�����ĵ�ms��

extern void xPortSysTickHandler(void);
//systick�жϷ�����,ʹ��OSʱ�õ�
void SysTick_Handler(void)
{  
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)//ϵͳ�Ѿ�����
    {
        xPortSysTickHandler();	
    }
}
			   
//SYSCLK:ϵͳʱ��Ƶ��
void delay_init(void)
{
    memset(SysTick, 0, sizeof(SysTick_Type));
    uint8_t freq = FL_RCC_GetSystemClockFreq() / 1000000;
	uint32_t reload;
	fac_us = freq;						    //�����Ƿ�ʹ��OS,fac_us����Ҫʹ��
	reload = freq;					        //ÿ���ӵļ������� ��λΪK	   
	reload *= 1000000 / configTICK_RATE_HZ;		//����configTICK_RATE_HZ�趨���ʱ��
											//reloadΪ24λ�Ĵ���,���ֵ:16777216,��180M��,Լ��0.745s����	
	fac_ms = 1000 / configTICK_RATE_HZ;			//����OS������ʱ�����ٵ�λ
    
    NVIC_SetPriority(SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL); /* set Priority for Systick Interrupt */
	SysTick->LOAD = reload; 					//ÿ1/configTICK_RATE_HZ��һ��	
    SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
               SysTick_CTRL_TICKINT_Msk   |
               SysTick_CTRL_ENABLE_Msk;                         /* Enable SysTick IRQ and SysTick Timer */
}								    

//��ʱnus
//nus:Ҫ��ʱ��us��.	
//nus:0~190887435(���ֵ��2^32/fac_us@fac_us=22.5)	    								   
void delay_us(uint32_t nus)
{		
	uint32_t ticks;
	uint32_t told, tnow, tcnt = 0;
	uint32_t reload = SysTick->LOAD;				//LOAD��ֵ	    	 
	ticks = nus * fac_us; 						//��Ҫ�Ľ����� 
	told = SysTick->VAL;        				//�ս���ʱ�ļ�����ֵ
	while (1)
	{
		tnow = SysTick->VAL;	
		if (tnow != told)
		{	    
			if (tnow < told) tcnt += told - tnow;	//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else tcnt += reload - tnow + told;	    
			told = tnow;
			if (tcnt >= ticks) break;			//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}  
	};									    
}  
	
//��ʱnms,�������������
//nms:Ҫ��ʱ��ms��
//nms:0~65535
void delay_ms(uint32_t nms)
{	
	if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)//ϵͳ�Ѿ�����
	{		
		if (nms >= fac_ms)						//��ʱ��ʱ�����OS������ʱ������ 
		{ 
   			vTaskDelay(nms / fac_ms);	 		//FreeRTOS��ʱ
		}
		nms %= fac_ms;						//OS�Ѿ��޷��ṩ��ôС����ʱ��,������ͨ��ʽ��ʱ    
	}
	delay_us((uint32_t)(nms * 1000));				//��ͨ��ʽ��ʱ
}

//��ʱnms,���������������
//nms:Ҫ��ʱ��ms��
void delay_xms(uint32_t nms)
{
	uint32_t i;
	for (i = 0; i < nms; i++) delay_us(1000);
}
