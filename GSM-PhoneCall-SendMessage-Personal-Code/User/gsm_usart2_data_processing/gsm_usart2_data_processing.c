#include "gsm_usart2_data_processing.h"
#include "bsp_gsm_usart2.h"
#include "bsp_systick.h"
#include "bsp_debug_usart1.h" 
#include "string.h"

extern char  USART2_RX_String[50];//���USART2���ڽ��յ������ݵ�����
static uint8_t Restart_Counter = 0;//������������ʼֵΪ0

/*GSM��ʼ������*/
uint8_t GSM_Init(void)
{
	SysTick_Delay_ms(1000);//�ȴ�GSMģ�鿪��
	GSM_USART2_Send("AT+CGMM\r");
	SysTick_Delay_ms(100);//�ȴ�USART2_RX��������
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


/*GSM�绰������*/
void GSM_Call(char *USART1_RX_String)//���GSMģ����Ӧ
{
	USART2_RX_Clean();//���USART2_RX_String[50]�����е�����
	
  GSM_USART2_Send("ATD");//����GSM���������ɾ��
  GSM_USART2_Send(USART1_RX_String);//����GSM���������ɾ��
  GSM_USART2_Send(";\r");//����GSM���������ɾ��
	SysTick_Delay_ms(100);
	if( strstr(USART2_RX_String,"OK") == NULL)//����Ƿ���OK����֡����
	{
		printf("�绰����ʧ�ܣ�\n");
		GSM_Restart();//����GSMģ��
	}
	else
	{
		printf("�绰����ɹ���\n");
	}
	USART2_RX_Clean();//���USART2_RX_String[50]�����е�����
}


/*GSM���ŷ��ͺ���*/
char end = 0x1A;//���Ͷ��Ž�������
void GSM_Send(char *Phone_Num,char *Content_Message)
{
	char CmdBuff[50];//���ָ������
	
  USART2_RX_Clean();//���USART2_RX_String[50]�����е�����
			
  GSM_USART2_Send("AT+CSCS=\"GSM\"\r");
	SysTick_Delay_ms(100);
	if( strstr(USART2_RX_String,"OK") == NULL)//����Ƿ���OK����֡����
	{
		printf("����AT+CSCSָ��ʧ��\n");
	}
	USART2_RX_Clean();//���USART2_RX_String[50]�����е�����
	
	
	GSM_USART2_Send("AT+CMGF=1\r");
	SysTick_Delay_ms(100);
	if( strstr(USART2_RX_String,"OK") == NULL)//����Ƿ���OK����֡����
	{
		printf("����AT+CMGFָ��ʧ��\n");
	}
	USART2_RX_Clean();//���USART2_RX_String[50]�����е�����
	
	
	sprintf( CmdBuff,"AT+CMGS=\"%s\"\r",Phone_Num);
	GSM_USART2_Send(CmdBuff);
	SysTick_Delay_ms(200);//�ȴ�USART2_RX��������
	if( strstr(USART2_RX_String,">") == NULL)//����Ƿ���>����֡����
	{
		printf("����AT+CMGSָ��ʧ��\n");
	}
	USART2_RX_Clean();//���USART2_RX_String[50]�����е�����
	
	
	GSM_USART2_Send(Content_Message);//���Ͷ�������
	GSM_USART2_Send( &end );//���ͽ���ָ��
	SysTick_Delay_ms(5000);//�ȴ�USART2_RX��������
	if( strstr(USART2_RX_String,"OK") == NULL)//����Ƿ���OK����֡����
	{
		printf("���Ͷ���ʧ��\n");
		GSM_Restart();//����GSMģ��
	}
	USART2_RX_Clean();//���USART2_RX_String[50]�����е�����
}

/*���SIM�������Ƿ��쳣����*/
uint8_t SIM_Check(void)
{
	USART2_RX_Clean();//���USART2_RX_String[50]�����е�����
  GSM_USART2_Send("AT+CNUM\r");//����GSM���������ɾ��
	SysTick_Delay_ms(100);
	if( strstr(USART2_RX_String,"OK") == NULL)//����Ƿ���OK����֡����
	{
		printf("SIM����������쳣����\n");//�������ɾ����!
		USART2_RX_Clean();//���USART2_RX_String[50]�����е�����
		return 0;
	}
	else
	{
		USART2_RX_Clean();//���USART2_RX_String[50]�����е�����
		return 1;
	}
}


/*����ź��Ƿ���������*/
uint8_t Signal_Check(void)
{
	USART2_RX_Clean();//���USART2_RX_String[50]�����е�����
  GSM_USART2_Send("AT+CSQ\r");//����GSM���������ɾ��
	SysTick_Delay_ms(100);
	if( strstr(USART2_RX_String,"OK") == NULL)//����Ƿ���OK����֡����
	{
		printf("�źŴ����쳣����\n");//�������ɾ����!
		USART2_RX_Clean();//���USART2_RX_String[50]�����е�����
		return 0;
	}
	else
	{
		USART2_RX_Clean();//���USART2_RX_String[50]�����е�����
		return 1;
	}
}

void GSM_Restart(void)
{
	USART2_RX_Clean();//���USART2_RX_String[50]�����е�����
  GSM_USART2_Send("AT+CFUN\r");//����GSM���������ɾ��
	SysTick_Delay_ms(100);
	if( strstr(USART2_RX_String,"OK") == NULL)//����Ƿ���OK����֡����
	{
		printf("�����쳣����\n");
		USART2_RX_Clean();//���USART2_RX_String[50]�����е�����
		Restart_Counter++;
		if(Restart_Counter == 5)//��GSMģ����������Ϊ5��ʱ����������
		{
			Restart_Counter = 0;//������0
			printf("��������GSM�������ʧ��\n");
		}
	}
	else
	{
		printf("�����ɹ�����\n");
		USART2_RX_Clean();//���USART2_RX_String[50]�����е�����
	}
}

