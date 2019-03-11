#include "bsp_debug_usart1.h"
#include "call_send_mode_change.h"

/*�����ж����ȼ�*/
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Ƕ�������жϿ�������ѡ�� */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* ����USARTΪ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
  /* �������ȼ�*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* �����ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* ʹ���ж� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* ��ʼ������NVIC */
  NVIC_Init(&NVIC_InitStructure);
}


/*��ʼ��USART1����*/
void USART1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// �򿪴���GPIO��ʱ��
	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);
	
	// �򿪴��������ʱ��
	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);

	// ��USART Tx��GPIO����Ϊ���츴��ģʽ
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  // ��USART Rx��GPIO����Ϊ��������ģʽ
	GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// ���ô��ڵĹ�������
	// ���ò�����
	USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
	// ���� �������ֳ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// ����ֹͣλ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// ����У��λ��ͬ��ͨ��ʱ��ʹ�ã���������Ϊ USART_Parity_No
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// ����Ӳ�������ƣ�ͬ��ͨ��ʱ��ʹ�ã���������Ϊ USART_HardwareFlowControl_None
	USART_InitStructure.USART_HardwareFlowControl = 
	USART_HardwareFlowControl_None;
	// ���ù���ģʽ���շ�һ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// ��ɴ��ڵĳ�ʼ������
	USART_Init(DEBUG_USART1, &USART_InitStructure);
	
	// �����ж����ȼ�����
	NVIC_Configuration();
	
	// ʹ�ܴ��ڽ����ж�
	USART_ITConfig(DEBUG_USART1, USART_IT_RXNE, ENABLE);	
	
	// ʹ�ܴ���
	USART_Cmd(DEBUG_USART1, ENABLE);	    
}


/*�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����*/
int fputc(int ch, FILE *f)
{
		//����һ���ֽ����ݵ�����
		USART_SendData(DEBUG_USART1, (uint8_t) ch);
		
		//�ȴ��������
		while (USART_GetFlagStatus(DEBUG_USART1, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}


char  USART1_RX_String[50];//���USART1���ڽ��յ������ݵ�����
volatile uint16_t USART1_Count = 0;//��¼���յ����ַ���������ʼֵλ0
volatile uint8_t  Receive_Over = 0;//�������ݽ�����ʶ������ʼ��Ϊ0

/*���ڽ����жϷ�����*/
void DEBUG_USART1_IRQHandler(void)
{
	if(USART_GetITStatus(DEBUG_USART1,USART_IT_RXNE)!=RESET)
	{		
		USART1_RX_String[USART1_Count] = USART_ReceiveData(DEBUG_USART1);//���մ��ڷ��͹������ַ�
		if(USART1_RX_String[USART1_Count] != '+')//�ж��Ƿ���յ��ո������־λ
		{
			USART1_Count++;
		}
    else
    {
			USART1_RX_String[USART1_Count] = '\0';//����ո������־λ
			Receive_Over = 1;//���������ݽ�����ʶ����ֵΪ1����ʾ�������ݽ���
		}			
	}	 
}


/*���USAET1���ڽ����ַ�������*/
void USART1_RX_Clean(void) 
{
	uint8_t b;
	for(b=0;b<50;b++)
	{
		USART1_RX_String[b] = '\0';
	}
	USART1_Count = 0;//�ָ���ʼֵ0
}


