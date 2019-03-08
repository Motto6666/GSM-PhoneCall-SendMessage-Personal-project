#include "bsp_gsm_usart2.h"
#include <stdarg.h>

/*�����ж����ȼ�*/
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  //Ƕ�������жϿ�������ѡ��
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  //����USARTΪ�ж�Դ
  NVIC_InitStructure.NVIC_IRQChannel = GSM_USART2_IRQ;
  //�������ȼ�
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  //�����ȼ�
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  //ʹ���ж�
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  //��ʼ������NVIC
  NVIC_Init(&NVIC_InitStructure);
}


/*��ʼ��USART2����*/
void GSM_USART2_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// �򿪴���GPIO��ʱ��
	GSM_USART2_GPIO_APBxClkCmd(GSM_USART2_GPIO_CLK, ENABLE);
	
	// �򿪴��������ʱ��
	GSM_USART2_APBxClkCmd(GSM_USART2_CLK, ENABLE);

	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = GSM_USART2_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GSM_USART2_TX_GPIO_PORT, &GPIO_InitStructure);

  // ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = GSM_USART2_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GSM_USART2_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure.USART_BaudRate = GSM_USART2_BAUDRATE;
	// ���� �������ֳ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// ����ֹͣλ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// ����У��λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// ����Ӳ��������
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// ���ù���ģʽ���շ�һ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// ��ɴ��ڵĳ�ʼ������
	USART_Init(GSM_USART2, &USART_InitStructure);
	
	// �����ж����ȼ�����
	NVIC_Configuration();
	
	// ʹ�ܴ��ڽ����ж�
	USART_ITConfig(GSM_USART2, USART_IT_RXNE, ENABLE);	
	
	// ʹ�ܴ���
	USART_Cmd(GSM_USART2, ENABLE);		
    
}


/*�����ַ���*/
void GSM_USART2_Send(char * str)
{    
  while(*str)  
  {        
		USART_SendData(GSM_USART2, *str++);  		          
		while(USART_GetFlagStatus(GSM_USART2, USART_FLAG_TC) == RESET);    
	}
}

volatile char  USART2_RX_String[50];//���USART2���ڽ��յ������ݵ�����
volatile uint16_t USART2_Count = 0;//��¼���յ����ַ���������ʼֵλ0
/*USART2�����жϷ�����*/
void GSM_USART2_IRQHandler(void)
{
	if(USART_GetITStatus(GSM_USART2,USART_IT_RXNE)!=RESET)
	{		
		USART2_RX_String[USART2_Count] = USART_ReceiveData(GSM_USART2);
    USART2_Count++;
    //USART2_RX_Clean();//���USAET1���ڽ����ַ������棬�����USART1_RX_String[50]�е�����
	    
	}	 
}


/*���USAET2���ڽ����ַ�������*/
void USART2_RX_Clean(void)
{
	uint8_t b;
	for(b=0;b<50;b++)
	{
		USART2_RX_String[b] = '\0';
	}
	USART2_Count = 0;//�ָ���ʼֵ0
}


