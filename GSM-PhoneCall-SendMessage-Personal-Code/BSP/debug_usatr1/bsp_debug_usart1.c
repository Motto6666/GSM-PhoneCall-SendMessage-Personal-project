#include "bsp_debug_usart1.h"
#include "string.h"

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


///�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ����� */
		USART_SendData(DEBUG_USART1, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(DEBUG_USART1, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}


static char  USART1_RX_String[50];//���USART1���ڽ��յ������ݵ�����
volatile uint16_t USART1_Count = 0;//��¼���յ����ַ���������ʼֵλ0
const char CallPhone_String[50]   = {"CallPhone"};//����绰�ַ�����ʶ��
const char SendMessage_String[50] = {"SendMessage"};//���Ͷ����ַ�����ʶ��
volatile uint8_t Call_Send_Order = 0;//����绰�ͷ��Ͷ��ű�ʶ������ʼֵΪ'0'
volatile char Phone_Num[50];//��ŵ绰���������
extern volatile uint8_t Mode;//����绰�ͷ��Ͷ���ģʽ����ʼֵΪ0

// �����жϷ�����
void DEBUG_USART1_IRQHandler(void)
{
	if(USART_GetITStatus(DEBUG_USART1,USART_IT_RXNE)!=RESET)
	{		
		USART1_RX_String[USART1_Count] = USART_ReceiveData(DEBUG_USART1);//���մ��ڷ��͹������ַ�
		if(USART1_RX_String[USART1_Count] != ' ')//�ж��Ƿ���յ��ո������־λ
		{
			USART1_Count++;
		}
    else
    {
			USART1_RX_String[USART1_Count] = '\0';//����ո������־λ
			
			if(strcmp(CallPhone_String,USART1_RX_String) == 0)//�ж�USART1���ڽ��յ��������Ƿ�Ϊ����绰�ַ�����ʶ��
			{
				Call_Send_Order = CALL_PHONE_Prepare;
			}
			
			if(strcmp(SendMessage_String,USART1_RX_String) == 0)//�ж�USART1���ڽ��յ��������Ƿ�Ϊ���Ͷ����ַ�����ʶ��
			{
				Call_Send_Order = SEND_MESSAGE_Prepare;
			}
			
			if((strcmp(SendMessage_String,USART1_RX_String) != 0 )&& (strcmp(CallPhone_String,USART1_RX_String) != 0))
			{
				if(strlen(USART1_RX_String) == 11)//�ж�����ĵ绰�����Ƿ�Ϊ11λ
				{
					switch(Mode)
					{
						case CALL_Already_Prepare:
						{
							Mode = 0;//ģʽ�ָ������ֵ�������ظ�ִ��
							Call_Send_Order = CALLING_PHONE;//�л�����绰״̬
						}break;
						
						case SEND_Already_Prepare:
						{
							Mode = 0;//ģʽ�ָ������ֵ�������ظ�ִ��
							Call_Send_Order = SENDING_MESSAGE;//�л����Ͷ���״̬
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
			USART1_RX_Clean();//���USAET1���ڽ����ַ������棬�����USART1_RX_String[50]�е�����
		}			
	}	 
}

void USART1_RX_Clean(void) //���USAET1���ڽ����ַ�������
{
	uint8_t b;
	for(b=0;b<50;b++)
	{
		USART1_RX_String[b] = '\0';
	}
	USART1_Count = 0;//�ָ���ʼֵ0
}


