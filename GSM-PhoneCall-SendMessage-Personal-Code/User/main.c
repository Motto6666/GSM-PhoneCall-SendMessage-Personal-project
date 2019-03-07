#include "stm32f10x.h"
#include "bsp_debug_usart1.h"
#include "call_send_mode_change.h"
#include "stdio.h"

extern volatile uint8_t Call_Send_Order;//拨打电话和发送短信标识符，初始值为0
extern volatile uint8_t Mode;//拨打电话和发送短信模式，初始值为0

int main(void)
{	
  USART1_Config();//初始化USART1串口
	printf("\r\n提示：输入CallPhone为拨打电话，输入SendMessage为发送短信\r\n");
	printf("\r\n注意：字符串后需要添加空格！！\r\n");
  while(1)
	{
		switch(Call_Send_Order)
		{
			case CALL_PHONE_Prepare:
			{
				printf("\r\n请输入电话号码：\r\n");
				Mode = CALL_Already_Prepare;//切换准备拨打电话模式
				Call_Send_Order = NONE;//避免重复操作
			}break;
			
			case CALLING_PHONE:
			{ 
				printf("\r\n正在拨打您输入的电话....\r\n");
				Call_Send_Order = NONE;//避免重复操作
			}break;
			
			case SEND_MESSAGE_Prepare:
			{
				printf("\r\n请输入短信电话号码：\r\n");
				Mode = SEND_Already_Prepare;//切换准备发送短信模式
				Call_Send_Order = NONE;//避免重复操作
			}break;
			
			case SENDING_MESSAGE:
			{ 
				printf("\r\n正在发送短信....\r\n");
				Call_Send_Order = NONE;//避免重复操作
			}break;
			
			case ERROR:
			{ 
				printf("\r\n输入错误的信息，请重新输入！\r\n");
				Call_Send_Order = NONE;//避免重复操作
			}break;
			
			default: break;
		}
	}	
}
