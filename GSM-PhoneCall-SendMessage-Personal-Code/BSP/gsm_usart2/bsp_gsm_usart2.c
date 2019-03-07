#include "bsp_gsm_usart2.h"

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


/*����һ���ַ�*/
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* ����һ���ֽ����ݵ�USART */
	USART_SendData(pUSARTx,ch);
		
	/* �ȴ��������ݼĴ���Ϊ�� */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}
/*�����ַ���*/
void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  
  /* �ȴ�������� */
  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
  {}
}


/*USART2�����жϷ�����*/
void GSM_USART2_IRQHandler(void)
{
  uint8_t ucTemp;
	if(USART_GetITStatus(GSM_USART2,USART_IT_RXNE)!=RESET)
	{		
		ucTemp = USART_ReceiveData(GSM_USART2);
    USART_SendData(GSM_USART2,ucTemp);    
	}	 
}



