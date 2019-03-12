#include "stm32f10x.h"
#include "bsp_debug_usart1.h"
#include "call_send_mode_change.h"
#include "bsp_gsm_usart2.h"
#include "gsm_usart2_data_processing.h"
#include "bsp_basetime.h"


extern volatile uint8_t Call_Send_Order;//拨打电话和发送短信标识符，初始值为0
extern volatile uint8_t Mode;//拨打电话和发送短信模式，初始值为0
extern char  USART1_RX_String[50];//存放USART1串口接收到的数据的数组
extern volatile uint8_t Receive_Over;//接收数据结束标识符，初始化为0
extern volatile uint8_t GSM_SysCheck;//GSM系统检测标志符，初始化为0
extern volatile uint32_t time;//时间计数,初始值为0

int main(void)
{	
  USART1_Config();//初始化USART1串口
	GSM_USART2_Config();//初始化USART2串口
	while(!GSM_Init());//初始化GSM模块
		
	printf("\r\n提示：输入CallPhone为拨打电话，输入SendMessage为发送短信\r\n");
	printf("\r\n注意：字符串后需要添加'+'符号！！\r\n");

	BASIC_TIM_Init();//初始化基本定时器
	
  while(1)
	{
		switch(Call_Send_Order)
		{
			case CALL_PHONE_PREPARE:
			{
				GSM_CHECK_OFF;//关闭GSM模块检测，避免在打电话的过程中中断执行GSM检测
				printf("\r\n请输入电话号码：\r\n");
				Mode = CALL_ALREADY_PREPARE;//切换准备拨打电话模式
				Call_Send_Order = NONE;//避免重复操作
			}break;
			
			case CALLING_PHONE:
			{
        GSM_CHECK_ON;//当拨打电话后，开启定时进行GSM模块检测
		    time = 0;//重新计时
				printf("\r\n正在拨打您输入的电话....\r\n");
				Call_Send_Order = NONE;//避免重复操作
			}break;
			
			case SEND_MESSAGE_PREPARE:
			{
				GSM_CHECK_OFF;//关闭GSM模块检测，避免在方式短信的过程中中断执行GSM检测
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
        GSM_CHECK_ON;//当发送短信结束后，开启定时进行GSM模块检测
		    time = 0;//重新计时				
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
		
		
		if(Receive_Over == 1)//USART1串口接收完毕
		{
			Receive_Over = 0;//接收数据结束标识符恢复到最初值，避免重复操作
			Call_Send_Mode_Change(USART1_RX_String);//判断输入的字符，作出GSM功能的切换
			USART1_RX_Clean();//清除USAET1串口接收字符串缓存，即清空USART1_RX_String[50]中的数据
		}
		
		
		if(GSM_SysCheck == 1)//每2min执行一次GSM系统检测
		{
			GSM_SysCheck = 0;//GSM系统检测标志符恢复到最初值，避免重复操作
			printf("执行GSM系统检测.....\n");
			if(SIM_Check() == 0 || Signal_Check() == 0)//检测GSM的SIM卡和工作信号是否存在问题
			{
				GSM_Restart();//重启GSM模块
			}
			else
			{
				printf("GSM模块工作正常\n");
			}	
		}
		
	}	
}
