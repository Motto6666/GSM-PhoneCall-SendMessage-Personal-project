#include "bsp_systick.h"

void SysTick_Delay_ms(uint32_t ms)
{
	uint32_t i;
	
	SysTick_Config(72000);//720000表示使用ms计时，改为72表示使用us计时（少用）
	
	for(i=0;i<ms;i++)
	{
		while( !( SysTick->CTRL & (1<<16) ));
	}
	
	/*失能systick*/
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

