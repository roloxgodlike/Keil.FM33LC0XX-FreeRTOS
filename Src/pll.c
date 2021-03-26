#include "pll.h"

static uint8_t fac_us=0;							//us延时倍乘数

void DelayInit(void)
{
    fac_us = FL_RCC_GetSystemClockFreq() / 1000000;//代表每个us需要的systick时钟数
    printf("fac_us: %u\n", fac_us);
}

void DelayUs(uint32_t us)
{
    us = us * fac_us;
    us = us > 16777216 ? 16777216 : us;
    SysTick->LOAD = us - 1;
    SysTick->VAL = 0;
    while (!((SysTick->CTRL >> 16) & 0x1));
}

void DelayMs(uint32_t ms)
{
    while (ms--)
        DelayUs(1000);
}


/**
  * @brief    
  * @rmtoll   PLLCR    OSEL    LL_RCC_GetPLLDigitalDomainOutput
  * @param    Source This parameter can be one of the following values:
  *           @arg @ref FL_RCC_PLL_CLK_SOURCE_RCHF
  *           @arg @ref FL_RCC_PLL_CLK_SOURCE_XTHF
  * @param    PLL_R This parameter can be one of the following values:
  *           @arg @ref FL_RCC_PLL_PSC_DIV1
  *           @arg @ref FL_RCC_PLL_PSC_DIV2
  *           @arg @ref FL_RCC_PLL_PSC_DIV4
  *           @arg @ref FL_RCC_PLL_PSC_DIV8
  *           @arg @ref FL_RCC_PLL_PSC_DIV12
  *           @arg @ref FL_RCC_PLL_PSC_DIV16
  *           @arg @ref FL_RCC_PLL_PSC_DIV24
  *           @arg @ref FL_RCC_PLL_PSC_DIV32
  * @param    PLL_DB This parameter can be Between parameters of the following values:
  *           @arg @ref 

  * @param    PLL_O This parameter can be one of the following values:
  *           @arg @ref FL_RCC_PLL_OUTPUT_X1
  *           @arg @ref FL_RCC_PLL_OUTPUT_X2
  * @retval   None
  */
void RCC_PLL_ConfigDomain_SYS(uint32_t Source, uint32_t PLL_R, uint32_t PLL_DB, uint32_t PLL_O)
{
  MODIFY_REG(RCC->PLLCR, RCC_PLLCR_DB_Msk | RCC_PLLCR_REFPRSC_Msk | RCC_PLLCR_OSEL_Msk | RCC_PLLCR_INSEL_Msk,
             (PLL_DB << RCC_PLLCR_DB_Pos) | PLL_R | PLL_O | Source);
}

void XTHFInit(void)    
{
    FL_GPIO_InitTypeDef gpioInitStruction;
    
    // GPIO Init
    gpioInitStruction.mode = FL_GPIO_MODE_ANALOG;
    gpioInitStruction.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    gpioInitStruction.pull = DISABLE;
    
    gpioInitStruction.pin = FL_GPIO_PIN_2 | FL_GPIO_PIN_3;
    FL_GPIO_Init(GPIOC, &gpioInitStruction);
    
    FL_RCC_XTHF_WriteDriverStrength(0xFFFFFFFF);
//    FL_FDET_EnableIT_XTHFFail();
//    FL_FDET_ClearFlag_XTHFFail();
    FL_RCC_XTHF_Enable();
    DelayMs(5);
//    printf("FL_RCC_XTHF_Enable-%u\n", FL_RCC_XTHF_IsEnabled());
//    printf("FL_FDET_IsEnabledIT_XTHFFail-%u\n", FL_FDET_IsEnabledIT_XTHFFail());
//    printf("FL_FDET_IsActiveFlag_XTHFFailOutput-%u\n", FL_FDET_IsActiveFlag_XTHFFailOutput());
//    printf("FL_FDET_IsActiveFlag_XTHFFail-%u\n", FL_FDET_IsActiveFlag_XTHFFail());
}

void SelXTHFToPLL(uint32_t prescaler, uint32_t multiplier)
{
    if (multiplier > FL_RCC_PLL_MUL_MAX) 
    {
//        printf("FL_RCC_PLL_MUL too large %u>%u\n", multiplier, FL_RCC_PLL_MUL_MAX);
        return;
    }
    
    DelayInit();
    XTHFInit();

    if (multiplier <= FL_RCC_PLL_MUL_32)
    {
        FL_FLASH_SetReadWait(FLASH, FL_FLASH_READ_WAIT_0CYCLE);
    }
    else if((multiplier > FL_RCC_PLL_MUL_32) && (multiplier <= FL_RCC_PLL_MUL_48))
    {
        FL_FLASH_SetReadWait(FLASH, FL_FLASH_READ_WAIT_1CYCLE);
    }
    else
    {
        FL_FLASH_SetReadWait(FLASH, FL_FLASH_READ_WAIT_2CYCLE); 
    }

//    RCC_PLL_ConfigDomain_SYS(FL_RCC_PLL_CLK_SOURCE_XTHF, FL_RCC_PLL_PSC_DIV8, multiplier, FL_RCC_PLL_OUTPUT_X1);
    FL_RCC_PLL_SetClockSource(FL_RCC_PLL_CLK_SOURCE_XTHF);
    FL_RCC_PLL_SetPrescaler(prescaler);
    FL_RCC_PLL_WriteMultiplier(multiplier);
    FL_RCC_PLL_SetOutputMultiplier(FL_RCC_PLL_OUTPUT_X1);
    FL_RCC_PLL_Enable();
    
    while(FL_RCC_IsActiveFlag_PLLReady() != SET);
    
//    printf("RCC->PLLCR: %u\n", RCC->PLLCR);
//    printf("FL_RCC_PLL_IsEnabled-%u\n", FL_RCC_PLL_IsEnabled());
//    printf("FL_RCC_PLL_GetClockSource-%u\n", FL_RCC_PLL_GetClockSource());
//    printf("FL_RCC_PLL_ReadMultiplier-%u\n", FL_RCC_PLL_ReadMultiplier());
//    printf("FL_RCC_PLL_GetPrescaler-%u\n", FL_RCC_PLL_GetPrescaler());
//    printf("FL_RCC_PLL_GetOutputMultiplier-%u\n", FL_RCC_PLL_GetOutputMultiplier());
    
    FL_FLASH_EnablePrefetch(FLASH);//使能Flash预取指
    
    FL_RCC_SetAHBPrescaler(FL_RCC_AHBCLK_PSC_DIV1);
    FL_RCC_SetAPB1Prescaler(FL_RCC_APB1CLK_PSC_DIV1);
    FL_RCC_SetAPB2Prescaler(FL_RCC_APB2CLK_PSC_DIV1);
    FL_RCC_SetSystemClockSource(FL_RCC_SYSTEM_CLK_SOURCE_PLL);
}

