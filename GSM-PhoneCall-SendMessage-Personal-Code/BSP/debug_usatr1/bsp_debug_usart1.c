#include "bsp_debug_usart1.h"
#include "call_send_mode_change.h"

/*配置中断优先级*/
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 嵌套向量中断控制器组选择 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* 配置USART为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
  /* 抢断优先级*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* 子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* 使能中断 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* 初始化配置NVIC */
  NVIC_Init(&NVIC_InitStructure);
}


/*初始化USART1外设*/
void USART1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// 打开串口GPIO的时钟
	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);
	
	// 打开串口外设的时钟
	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);

	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  // 将USART Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// 配置串口的工作参数
	// 配置波特率
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
	// 配置 针数据字长
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// 配置停止位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// 配置校验位，同步通信时才使用，所以配置为 USART_Parity_No
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// 配置硬件流控制，同步通信时才使用，所以配置为 USART_HardwareFlowControl_None
	USART_InitStructure.USART_HardwareFlowControl = 
	USART_HardwareFlowControl_None;
	// 配置工作模式，收发一起
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// 完成串口的初始化配置
	USART_Init(DEBUG_USART1, &USART_InitStructure);
	
	// 串口中断优先级配置
	NVIC_Configuration();
	
	// 使能串口接收中断
	USART_ITConfig(DEBUG_USART1, USART_IT_RXNE, ENABLE);	
	
	// 使能串口
	USART_Cmd(DEBUG_USART1, ENABLE);	    
}


/*重定向c库函数printf到串口，重定向后可使用printf函数*/
int fputc(int ch, FILE *f)
{
		//发送一个字节数据到串口
		USART_SendData(DEBUG_USART1, (uint8_t) ch);
		
		//等待发送完毕
		while (USART_GetFlagStatus(DEBUG_USART1, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}


static char  USART1_RX_String[50];//存放USART1串口接收到的数据的数组
volatile uint16_t USART1_Count = 0;//记录接收到的字符个数，初始值位0

/*串口接收中断服务函数*/
void DEBUG_USART1_IRQHandler(void)
{
	if(USART_GetITStatus(DEBUG_USART1,USART_IT_RXNE)!=RESET)
	{		
		USART1_RX_String[USART1_Count] = USART_ReceiveData(DEBUG_USART1);//接收串口发送过来的字符
		if(USART1_RX_String[USART1_Count] != '+')//判断是否接收到空格结束标志位
		{
			USART1_Count++;
		}
    else
    {
			USART1_RX_String[USART1_Count] = '\0';//清除空格结束标志位
			Call_Send_Mode_Change(USART1_RX_String);//判断输入的字符，作出GSM功能的切换
			USART1_RX_Clean();//清除USAET1串口接收字符串缓存，即清空USART1_RX_String[50]中的数据
		}			
	}	 
}


/*清除USAET1串口接收字符串缓存*/
void USART1_RX_Clean(void) 
{
	uint8_t b;
	for(b=0;b<50;b++)
	{
		USART1_RX_String[b] = '\0';
	}
	USART1_Count = 0;//恢复初始值0
}


