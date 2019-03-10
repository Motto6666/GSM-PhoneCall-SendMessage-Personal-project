#include "gsm_usart2_data_processing.h"
#include "bsp_gsm_usart2.h"
#include "bsp_systick.h"
#include "bsp_debug_usart1.h" 
#include "string.h"

extern char  USART2_RX_String[50];//存放USART2串口接收到的数据的数组
//static char Phone_Num_Str[50] = {"ATD18033156303;\r"};//存放电话号码的数组，调试完毕删除！！

/*GSM初始化函数*/
uint8_t GSM_Init(void)
{
	SysTick_Delay_ms(1000);//等待GSM模块开机
	GSM_USART2_Send("AT+CGMM\r");
	SysTick_Delay_ms(100);//等待USART2_RX接收数据
//	printf("%s\n",USART2_RX_String);//调试完毕删除！
//	printf("%c\n",USART2_RX_String[29]);//调试完毕删除�
	//if( USART2_RX_String[28] == 'O' && USART2_RX_String[29] == 'K')//判断是否接收到GSM发送过来的"OK"字�
	if( strstr(USART2_RX_String,"OK") != NULL)//判断是否接收到GSM发送过来的"OK"字符
	{
		printf("GSM模块响应成功\n");
		USART2_RX_Clean();//清除USART2_RX_String[50]数组中的数据
		return 1;
	}
	else
	{
		printf("GSM模块响应失败\n");
		USART2_RX_Clean();//清除USART2_RX_String[50]数组中的数据
	  return 0;
	}
}


/*GSM电话拨打函数*/
void GSM_Call(char *USART1_RX_String)//检查GSM模块响应
{
	USART2_RX_Clean();//清除USART2_RX_String[50]数组中的数据
	//GSM_USART2_Send("ATD18033156303;\r");//向GSM模块发送拨打电话指令
	
  GSM_USART2_Send("ATD");//调试GSM，调试完毕删除
  GSM_USART2_Send(USART1_RX_String);//调试GSM，调试完毕删除
  GSM_USART2_Send(";\r");//调试GSM，调试完毕删除

//	while(strstr( USART2_RX_String,"NO ANSWER") == NULL );//判断是否接收到GSM发送过来的"OK"字符
//	SysTick_Delay_ms(100);//等待USART2_RX接收数据
//	printf("%s\n",USART2_RX_String);//调试完毕删除！！
//	printf("%c\n",USART2_RX_String[0]);//调试完毕删除！！
//	if( USART2_RX_String[0] == 'O')
//	{
//		printf("电话拨打成功\n");
//		USART2_RX_Clean();//清除USART2_RX_String[50]数组中的数据
//	}
//	else
//	{
//		printf("电话拨打失败\n");
//		USART2_RX_Clean();//清除USART2_RX_String[50]数组中的数据
//	}
}


/*GSM短信发送函数*/
char end = 0x1A;//发送短信结束符号
void GSM_Send(char *Phone_Num,char *Content_Message)
{
	char CmdBuff[50];//存放指令数组
	
  USART2_RX_Clean();//清除USART2_RX_String[50]数组中的数据
			
  GSM_USART2_Send("AT+CSCS=\"GSM\"\r");
	SysTick_Delay_ms(100);
	while( strstr(USART2_RX_String,"OK") == NULL);//检测是否有OK数据帧返回
	printf("%s\n",USART2_RX_String);//调试完毕删除！!
	USART2_RX_Clean();//清除USART2_RX_String[50]数组中的数据
	
	GSM_USART2_Send("AT+CMGF=1\r");
	SysTick_Delay_ms(100);
	while( strstr(USART2_RX_String,"OK") == NULL);//检测是否有OK数据帧返回
	printf("%s\n",USART2_RX_String);//调试完毕删除！!
	USART2_RX_Clean();//清除USART2_RX_String[50]数组中的数据
	
	sprintf( CmdBuff,"AT+CMGS=\"%s\"\r",Phone_Num);
	GSM_USART2_Send(CmdBuff);
	SysTick_Delay_ms(200);//等待USART2_RX接收数据
	while( strstr(USART2_RX_String,">") == NULL);//检测是否有>数据帧返回
	printf("%s\n",USART2_RX_String);//调试完毕删除！!
	USART2_RX_Clean();//清除USART2_RX_String[50]数组中的数据
	
	GSM_USART2_Send(Content_Message);//发送短信类容
	
	GSM_USART2_Send( &end );//发送结束指令
	SysTick_Delay_ms(5000);//等待USART2_RX接收数据
	while( strstr(USART2_RX_String,"OK") == NULL);//检测是否有OK数据帧返回
	printf("%s\n",USART2_RX_String);//调试完毕删除！!
	USART2_RX_Clean();//清除USART2_RX_String[50]数组中的数据
}


