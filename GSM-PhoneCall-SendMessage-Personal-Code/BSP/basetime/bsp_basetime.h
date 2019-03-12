#ifndef __BSP_BASETIME_H
#define __BSP_BASETIME_H

#include "stm32f10x.h"

#define            BASIC_TIM                   TIM6
#define            BASIC_TIM_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            BASIC_TIM_CLK               RCC_APB1Periph_TIM6
#define            BASIC_TIM_Period            50000-1
#define            BASIC_TIM_Prescaler         71
#define            BASIC_TIM_IRQ               TIM6_IRQn
#define            BASIC_TIM_IRQHandler        TIM6_IRQHandler

#define            GSM_CHECK_ON                TIM_Cmd(BASIC_TIM, ENABLE);
#define            GSM_CHECK_OFF               TIM_Cmd(BASIC_TIM, DISABLE);

void BASIC_TIM_Init(void);

#endif /*__BSP_BASETIME_H*/
