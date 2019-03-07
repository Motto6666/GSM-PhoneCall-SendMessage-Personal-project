/*********************************
*改变GSM模块拨打电话和发送短信模式
**********************************/

#include "call_send_mode_change.h"
#include "string.h"
#include "bsp_debug_usart1.h"

const char CallPhone_String[50]   = {"CallPhone"};//拨打电话字符串标识符
const char SendMessage_String[50] = {"SendMessage"};//发送短信字符串标识符
volatile uint8_t Call_Send_Order = NONE;//拨打电话和发送短信标识符，初始值为'0'
static char Phone_Num_Str[50];//存放电话号码的数组
volatile uint8_t Mode = NONE;//拨打电话和发送短信模式，初始值为0

void Call_Send_Mode_Change(char *USART1_RX_String)
{
	if(strcmp(CallPhone_String,USART1_RX_String) == 0)//判断USART1串口接收到的数据是否为拨打电话字符串标识符
	{
		Call_Send_Order = CALL_PHONE_Prepare;
	}
			
	if(strcmp(SendMessage_String,USART1_RX_String) == 0)//判断USART1串口接收到的数据是否为发送短信字符串标识符
	{
		Call_Send_Order = SEND_MESSAGE_Prepare;
	}
			
	if((strcmp(SendMessage_String,USART1_RX_String) != 0 )&& (strcmp(CallPhone_String,USART1_RX_String) != 0))
	{
				if(strlen(USART1_RX_String) == 11)//判断输入的电话号码是否为11位
				{
					switch(Mode)
					{
						case CALL_Already_Prepare:
						{
							Mode = NONE;//模式恢复到最初值，避免重复执行
							Call_Send_Order = CALLING_PHONE;
							strcpy(Phone_Num_Str,USART1_RX_String);//将接收到的电话号码放入到Phone_Num_Str数组中
						}break;
						
						case SEND_Already_Prepare:
						{
							Mode = NONE;//模式恢复到最初值，避免重复执行
							Call_Send_Order = SENDING_MESSAGE;//切换发送短信状态
							strcpy(Phone_Num_Str,USART1_RX_String);
						}break;
						
						default:
            {
							Call_Send_Order = ERROR;
						}break;
							
					}
				}
				else
				{
					Call_Send_Order = ERROR;
				}
	}
}
