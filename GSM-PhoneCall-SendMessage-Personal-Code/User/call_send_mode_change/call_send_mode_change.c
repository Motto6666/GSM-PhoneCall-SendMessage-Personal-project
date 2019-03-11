/*********************************
*改变GSM模块拨打电话和发送短信模式
**********************************/

#include "call_send_mode_change.h"
#include "string.h"
#include "bsp_debug_usart1.h"//留着USART2头文件，调试GSM，调试完毕删除！！！
#include "bsp_gsm_usart2.h" //留着USART2头文件，调试GSM，调试完毕删除！！！
#include "gsm_usart2_data_processing.h"

const char CallPhone_String[50]   = {"CallPhone"};//拨打电话字符串标识符
const char SendMessage_String[50] = {"SendMessage"};//发送短信字符串标识符
volatile uint8_t Call_Send_Order = NONE;//拨打电话和发送短信标识符，初始值为'0'
volatile uint8_t Mode = NONE;//拨打电话和发送短信模式，初始值为0
char Phone_Num[50];//输入的记录电话号码


void Call_Send_Mode_Change(char *USART1_RX_String)
{
	//判断USART1串口接收到的数据是否为拨打电话字符串标识符
	if(strcmp(CallPhone_String,USART1_RX_String) == 0)
	{
		Call_Send_Order = CALL_PHONE_PREPARE;
	}
		
	//判断USART1串口接收到的数据是否为发送短信字符串标识符
	if(strcmp(SendMessage_String,USART1_RX_String) == 0)
	{
		Call_Send_Order = SEND_MESSAGE_PREPARE;
	}
			
	if((strcmp(SendMessage_String,USART1_RX_String) != 0 )&& (strcmp(CallPhone_String,USART1_RX_String) != 0))
	{
				if(strlen(USART1_RX_String) == 11)//判断输入的电话号码是否为11位
				{
					switch(Mode)
					{
						case CALL_ALREADY_PREPARE:
						{
							Mode = NONE;//模式恢复到最初值，避免重复执行
							Call_Send_Order = CALLING_PHONE;
							GSM_Call(USART1_RX_String);						
						}break;
						
						case SEND_ALREADY_PREPARE:
						{
							Call_Send_Order = CONTENT_MESSAGE;//切换输入英文短信内容状态
							strcpy(Phone_Num,USART1_RX_String);//将输入的电话号码复制到Phone_Num数组上
						}break;
						
						case SENDING_MESSAGE:
						{
							Mode = NONE;//模式恢复到最初值，避免重复执行
							Call_Send_Order = SENDING_MESSAGE;//切换发送短信状态
							printf("%s\n",Phone_Num);//调试使用，调试完毕删除
							printf("%s\n",USART1_RX_String);//调试使用，调试完毕删除
							GSM_Send(Phone_Num,USART1_RX_String);//发送短信
							PhoneNum_Clean(Phone_Num);//清除Phone_Num数组中的数据
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
	
	if(Mode == SENDING_MESSAGE)
	{
		Mode = NONE;//模式恢复到最初值，避免重复执行
		Call_Send_Order = SENDING_MESSAGE;//切换发送短信状态
		printf("%s\n",Phone_Num);//调试使用，调试完毕删除
		printf("%s\n",USART1_RX_String);//调试使用，调试完毕删除
		GSM_Send(Phone_Num,USART1_RX_String);//发送短信
	  PhoneNum_Clean(Phone_Num);//清除Phone_Num数组中的数据
	}
}

void PhoneNum_Clean(char *PhoneNum)
{
	uint8_t i;
	for(i=0;i<50;i++)
	{
		PhoneNum[i] = '\0';
	}
}
