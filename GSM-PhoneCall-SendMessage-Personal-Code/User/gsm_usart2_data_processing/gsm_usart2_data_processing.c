#include "gsm_usart2_data_processing.h"
#include "bsp_gsm_usart2.h"
#include "bsp_systick.h"
#include "bsp_debug_usart1.h" 
#include "string.h"

extern char  USART2_RX_String[50];//���USART2���ڽ��յ������ݵ�����
//static char Phone_Num_Str[50] = {"ATD18033156303;\r"};//��ŵ绰��������飬�������ɾ������

/*GSM��ʼ������*/
uint8_t GSM_Init(void)
{
	SysTick_Delay_ms(1000);//�ȴ�GSMģ�鿪��
	GSM_USART2_Send("AT+CGMM\r");
	SysTick_Delay_ms(100);//�ȴ�USART2_RX��������
//	printf("%s\n",USART2_RX_String);//�������ɾ����
//	printf("%c\n",USART2_RX_String[29]);//�������ɾ���
	//if( USART2_RX_String[28] == 'O' && USART2_RX_String[29] == 'K')//�ж��Ƿ���յ�GSM���͹�����"OK"�ַ
	if( strstr(USART2_RX_String,"OK") != NULL)//�ж��Ƿ���յ�GSM���͹�����"OK"�ַ�
	{
		printf("GSMģ����Ӧ�ɹ�\n");
		USART2_RX_Clean();//���USART2_RX_String[50]�����е�����
		return 1;
	}
	else
	{
		printf("GSMģ����Ӧʧ��\n");
		USART2_RX_Clean();//���USART2_RX_String[50]�����е�����
	  return 0;
	}
}


void GSM_Call(char *USART1_RX_String)//���GSMģ����Ӧ
{
	USART2_RX_Clean();//���USART2_RX_String[50]�����е�����
	//GSM_USART2_Send("ATD18033156303;\r");//��GSMģ�鷢�Ͳ���绰ָ��
	
  GSM_USART2_Send("ATD");//����GSM���������ɾ��
  GSM_USART2_Send(USART1_RX_String);//����GSM���������ɾ��
  GSM_USART2_Send(";\r");//����GSM���������ɾ��

//	while(strstr( USART2_RX_String,"NO ANSWER") == NULL );//�ж��Ƿ���յ�GSM���͹�����"OK"�ַ�
//	SysTick_Delay_ms(100);//�ȴ�USART2_RX��������
//	printf("%s\n",USART2_RX_String);//�������ɾ������
//	printf("%c\n",USART2_RX_String[0]);//�������ɾ������
//	if( USART2_RX_String[0] == 'O')
//	{
//		printf("�绰����ɹ�\n");
//		USART2_RX_Clean();//���USART2_RX_String[50]�����е�����
//	}
//	else
//	{
//		printf("�绰����ʧ��\n");
//		USART2_RX_Clean();//���USART2_RX_String[50]�����е�����
//	}
}


