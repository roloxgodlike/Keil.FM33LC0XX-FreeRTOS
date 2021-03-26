#ifndef __USER_INIT_H__
#define __USER_INIT_H__

#include "main.h"
#ifndef MFANG
#include <stdio.h>
#endif


#ifndef MFANG

#define LED0_GPIO   GPIOC
#define LED0_PIN    FL_GPIO_PIN_0

#define LED0_ON   FL_GPIO_ResetOutputPin(LED0_GPIO, LED0_PIN)
#define LED0_OFF  FL_GPIO_SetOutputPin(LED0_GPIO, LED0_PIN)
#define LED0_TOG  FL_GPIO_ToggleOutputPin(LED0_GPIO, LED0_PIN)

#endif

void UserInit(void);
void ClockInit(uint32_t clock);
void SystickInit(void);
void LedInit(void);
void DebugUartInit(uint32_t baud);

void FoutInit(void);

void delay_init(void);
void delay_us(uint32_t nus);
void delay_ms(uint32_t nms);
void delay_xms(uint32_t nms);

#endif
