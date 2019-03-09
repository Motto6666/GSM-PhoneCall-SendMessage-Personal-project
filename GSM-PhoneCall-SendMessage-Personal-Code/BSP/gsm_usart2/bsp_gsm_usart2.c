#include "bsp_gsm_usart2.h"
#include <stdarg.h>

/*配置中断优先级*/
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  //嵌套向量中断控制器组选择
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  //配置USART为中断源
  NVIC_InitStructure.NVIC_IRQChannel = GSM_USART2_IRQ;
  //抢断优先级
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  //子优先级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  //使能中断
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  //初始化配置NVIC
  NVIC_Init(&NVIC_InitStructure);
}


/*初始化USART2外设*/
void GSM_USART2_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// 打开串口GPIO的时钟
	GSM_USART2_GPIO_APBxClkCmd(GSM_USART2_GPIO_CLK, ENABLE);
	
	// 打开串口外设的时钟
	GSM_USART2_APBxClkCmd(GSM_USART2_CLK, ENABLE);

	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = GSM_USART2_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GSM_USART2_TX_GPIO_PORT, &GPIO_InitStructure);

  // 将USART Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = GSM_USART2_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GSM_USART2_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// 配置串口的工作参数
	// 配置波特率
	USART_InitStructure.USART_BaudRate = GSM_USART2_BAUDRATE;
	// 配置 针数据字长
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// 配置停止位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// 配置校验位
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// 配置硬件流控制
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// 配置工作模式，收发一起
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// 完成串口的初始化配置
	USART_Init(GSM_USART2, &USART_InitStructure);
	
	// 串口中断优先级配置
	NVIC_Configuration();
	
	// 使能串口接收中断
	USART_ITConfig(GSM_USART2, USART_IT_RXNE, ENABLE);	
	
	// 使能串口
	USART_Cmd(GSM_USART2, ENABLE);		
    
}


/*发送字符串*/
void GSM_USART2_Send(char * str)
{    
  while(*str)  
  {        
		USART_SendData(GSM_USART2, *str++);  		          
		while(USART_GetFlagStatus(GSM_USART2, USART_FLAG_TC) == RESET);    
	}
}

char  USART2_RX_String[50];//存放USART2串口接收到的数据的数组
volatile uint16_t USART2_Count = 0;//记录接收到的字符个数，初始值位0
/*USART2串口中断服务函数*/
void GSM_USART2_IRQHandler(void)
{
	if(USART_GetITStatus(GSM_USART2,USART_IT_RXNE)!=RESET)
	{		
		USART2_RX_String[USART2_Count] = USART_ReceiveData(GSM_USART2);
    USART2_Count++;
    //USART2_RX_Clean();//清除USAET1串口接收字符串缓存，即清空USART1_RX_String[50]中的数据
	    
	}	 
}


/*清除USAET2串口接收字符串缓存*/
void USART2_RX_Clean(void)
{
	uint8_t b;
	for(b=0;b<50;b++)
	{
		USART2_RX_String[b] = '\0';
	}
	USART2_Count = 0;//恢复初始值0
}


