#include "bsp_basetime.h"
#include "bsp_debug_usart1.h"//测试使用，使用完毕删除

/*中断优先级配置*/
static void BASIC_TIM_NVIC_Config(void)//移植时可以根据实际情况作参数修改
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//移植时可以根据实际情况作参数修改	
		// 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = BASIC_TIM_IRQ ;	//移植时可以根据实际情况作参数修改
		// 设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	 //移植时可以根据实际情况作参数修改
	  // 设置抢占优先级为3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	//移植时可以根据实际情况作参数修改
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*配置基本定时器外设*/
static void BASIC_TIM_Mode_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
		// 开启定时器时钟,即内部时钟CK_INT=72M,内部时钟可固定
    BASIC_TIM_APBxClock_FUN(BASIC_TIM_CLK, ENABLE);//移植时可以根据实际情况作修改
	
		// 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
    TIM_TimeBaseStructure.TIM_Period = BASIC_TIM_Period;//移植时可以根据实际情况作修改(BASIC_TIM_Period = 1000-1)重点理解！！

	  // 时钟预分频数= CK_INT /（BASIC_TIM_Prescaler+1） = 72M / 71+1 = 1M，即中断周期 T = BASIC_TIM_Period * (1/1000000) = 1000 * (1/1000000) = 1ms 重点理解！！
    TIM_TimeBaseStructure.TIM_Prescaler= BASIC_TIM_Prescaler;//移植时可以根据实际情况作修改(BASIC_TIM_Prescaler= 71)
		
	  // 初始化定时器
    TIM_TimeBaseInit(BASIC_TIM, &TIM_TimeBaseStructure);//移植时可以根据实际情况作修改
		
		// 清除计数器中断标志位
    TIM_ClearFlag(BASIC_TIM, TIM_FLAG_Update);//移植时可以根据实际情况作修改
	  
		// 开启计数器中断
    TIM_ITConfig(BASIC_TIM,TIM_IT_Update,ENABLE);//移植时可以根据实际情况作修改
		
		// 使能定时器
    TIM_Cmd(BASIC_TIM, ENABLE);	//移植时可以根据实际情况作修改
}

void BASIC_TIM_Init(void)
{
	BASIC_TIM_NVIC_Config();
	BASIC_TIM_Mode_Config();
}


/*中断服务函数*/
volatile uint32_t time;//时间计数
volatile uint8_t  GSM_SysCheck = 0;//GSM系统检测标志符，初始化为0
void  BASIC_TIM_IRQHandler (void) //中断服务函数,50ms中断一次
{
	if ( TIM_GetITStatus( BASIC_TIM, TIM_IT_Update) != RESET ) 
	{	
		time++;
		TIM_ClearITPendingBit(BASIC_TIM , TIM_FLAG_Update); 
    if(time == 1200)//1min检测GSM模块是否存在异常
    {
			time = 0;//time清0，重新计时
			GSM_SysCheck = 1;
		}			
	}		 	
}
