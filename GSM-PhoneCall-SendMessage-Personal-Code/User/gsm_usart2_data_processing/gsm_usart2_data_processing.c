#include "gsm_usart2_data_processing.h"
#include "bsp_gsm_usart2.h"
#include "bsp_systick.h"
#include "bsp_debug_usart1.h" //调试使用，调试完毕删除

const char GSM_OK_Cmd[]  = {"OK"};//GSM发送的OK指令
extern volatile char  USART2_RX_String[50];//存放USART2串口接收到的数据的数组
//static char Phone_Num_Str[50] = {"ATD18033156303;\r"};//存放电话号码的数组，调试完毕删除！！

uint8_t GSM_Check_Response(void)//检查GSM模块响应
{
	GSM_USART2_Send("AT+CGMM\r");//调试GSM，调试完毕删除
	SysTick_Delay_ms(2000);//延时10ms，等待接收“OK”指令
	printf("%s\n\n",USART2_RX_String);
	if( USART2_RX_String[0] == GSM_OK_Cmd[0] && USART2_RX_String[1] == GSM_OK_Cmd[1])
	{
		USART2_RX_Clean();//清除USART2_RX_String[50]数组中的内存
		return 1;
	}
	USART2_RX_Clean();//清除USART2_RX_String[50]数组中的内存
	return 0;
}

void GSM_Call_Cmd(void)
{
  if(GSM_Check_Response() == 1)
	{
		printf("GSM模块响应成功");
	}
	else
	{
		printf("GSM模块响应失败");
	}
//  GSM_USART2_Send("ATD");//调试GSM，调试完毕删除
//	GSM_USART2_Send(USART1_RX_String);//调试GSM，调试完毕删除
//	GSM_USART2_Send(";\r");//调试GSM，调试完毕删除
}
