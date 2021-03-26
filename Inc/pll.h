#ifndef __PLL_H__
#define __PLL_H__

#include "main.h"
#ifndef MFANG
#include <stdio.h>
#endif

#define FL_RCC_PLL_MUL_24 (0x17U << RCC_PLLCR_DB_Pos)//±¶Æµ23
#define FL_RCC_PLL_MUL_MAX (0x3fU << RCC_PLLCR_DB_Pos)//±¶Æµ×î´ó64

void SelXTHFToPLL(uint32_t prescaler, uint32_t multiplier);

#endif
