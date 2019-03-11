#include "stm32f10x.h"
#include "bsp_debug_usart1.h"
#include "call_send_mode_change.h"
#include "stdio.h"
#include "bsp_gsm_usart2.h"
#include "gsm_usart2_data_processing.h"

//#include "bsp_systick.h"//调试完毕删除！!
//#include "string.h"//调试完毕删除！!
//extern char  USART2_RX_String[50];//调试完毕删除！!
//char end = 0x1A;//调试完毕删除！!

extern volatile uint8_t Call_Send_Order;//拨打电话和发送短信标识符，初始值为0
extern volatile uint8_t Mode;//拨打电话和发送短信模式，初始值为0
extern char  USART1_RX_String[50];//存放USART1串口接收到的数据的数组
extern volatile uint8_t  Receive_Over;//接收数据结束标识符，初始化为0


int main(void)
{	
  USART1_Config();//初始化USART1串口
	GSM_USART2_Config();//初始化USART2串口
	
	while(!GSM_Init());//初始化GSM模块
		
	printf("\r\n提示：输入CallPhone为拨打电话，输入SendMessage为发送短信\r\n");
	printf("\r\n注意：字符串后需要添加'+'符号！！\r\n");

//	/*检测SIM卡插入是否异常*/
//	USART2_RX_Clean();//清除USART2_RX_String[50]数组中的数据
//  GSM_USART2_Send("AT+CNUM\r");//调试GSM，调试完毕删除
//	SysTick_Delay_ms(100);
//  printf("%s\n",USART2_RX_String);//调试完毕删除！!
//	USART2_RX_Clean();//清除USART2_RX_String[50]数组中的数据
//	�
//	/*检测信号*/
//	USART2_RX_Clean();//清除USART2_RX_String[50]数组中的数据
//  GSM_USART2_Send("AT+CSQ\r");//调试GSM，调试完毕删除
//	SysTick_Delay_ms(100);
//  printf("%s\n",USART2_RX_String);//调试完毕删除！!
//	USART2_RX_Clean();//清除USART2_RX_String[50]数组中的数据
	
	
	
  while(1)
	{
		switch(Call_Send_Order)
		{
			case CALL_PHONE_PREPARE:
			{
				printf("\r\n请输入电话号码：\r\n");
				Mode = CALL_ALREADY_PREPARE;//切换准备拨打电话模式
				Call_Send_Order = NONE;//避免重复操作
			}break;
			
			case CALLING_PHONE:
			{ 
				printf("\r\n正在拨打您输入的电话....\r\n");
				Call_Send_Order = NONE;//避免重复操作
			}break;
			
			case SEND_MESSAGE_PREPARE:
			{
				printf("\r\n请输入短信电话号码：\r\n");
				Mode = SEND_ALREADY_PREPARE;//切换准备发送短信模式
				Call_Send_Order = NONE;//避免重复操作
			}break;
			
			case CONTENT_MESSAGE:
			{
				printf("\r\n请输入英文短信内容：\r\n");
				Mode = SENDING_MESSAGE;//切换为发送短信模式
				Call_Send_Order = NONE;//避免重复操作
			}break;
			
			case SENDING_MESSAGE:
			{ 
				printf("\r\n发送短信短信完毕\r\n");
				Call_Send_Order = NONE;//避免重复操作
			}break;
			
			case ERROR:
			{ 
				printf("\r\n输入错误的信息，请重新输入！\r\n");
				Call_Send_Order = NONE;//避免重复操作
			}break;
			
			default: break;
		}
		
		if(Receive_Over == 1)
		{
			Receive_Over = 0;//接收数据结束标识符恢复到最初值，避免重复操作
			Call_Send_Mode_Change(USART1_RX_String);//判断输入的字符，作出GSM功能的切换
			USART1_RX_Clean();//清除USAET1串口接收字符串缓存，即清空USART1_RX_String[50]中的数据
		}
	}	
}
