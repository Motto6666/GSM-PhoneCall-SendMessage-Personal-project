/*********************************
*�ı�GSMģ�鲦��绰�ͷ��Ͷ���ģʽ
**********************************/

#include "call_send_mode_change.h"
#include "string.h"
#include "gsm_usart2_data_processing.h"

const char CallPhone_String[50]   = {"CallPhone"};//����绰�ַ�����ʶ��
const char SendMessage_String[50] = {"SendMessage"};//���Ͷ����ַ�����ʶ��
volatile uint8_t Call_Send_Order = NONE;//����绰�ͷ��Ͷ��ű�ʶ������ʼֵΪ'0'
volatile uint8_t Mode = NONE;//����绰�ͷ��Ͷ���ģʽ����ʼֵΪ0
char Phone_Num[50];//����ļ�¼�绰����


void Call_Send_Mode_Change(char *USART1_RX_String)
{
	//�ж�USART1���ڽ��յ��������Ƿ�Ϊ����绰�ַ�����ʶ��
	if(strcmp(CallPhone_String,USART1_RX_String) == 0)
	{
		Call_Send_Order = CALL_PHONE_PREPARE;
	}
		
	//�ж�USART1���ڽ��յ��������Ƿ�Ϊ���Ͷ����ַ�����ʶ��
	if(strcmp(SendMessage_String,USART1_RX_String) == 0)
	{
		Call_Send_Order = SEND_MESSAGE_PREPARE;
	}
			
	if((strcmp(SendMessage_String,USART1_RX_String) != 0 )&& (strcmp(CallPhone_String,USART1_RX_String) != 0))
	{
				if(strlen(USART1_RX_String) == 11 || strlen(USART1_RX_String) == 7)//�ж�����ĵ绰�����Ƿ�Ϊ11λ��7λ
				{
					switch(Mode)
					{
						case CALL_ALREADY_PREPARE:
						{
							Mode = NONE;//ģʽ�ָ������ֵ�������ظ�ִ��
							Call_Send_Order = CALLING_PHONE;
							GSM_Call(USART1_RX_String);						
						}break;
						
						case SEND_ALREADY_PREPARE:
						{
							Call_Send_Order = CONTENT_MESSAGE;//�л�����Ӣ�Ķ�������״̬
							strcpy(Phone_Num,USART1_RX_String);//������ĵ绰���븴�Ƶ�Phone_Num������
						}break;
						
						case SENDING_MESSAGE:
						{
							Mode = NONE;//ģʽ�ָ������ֵ�������ظ�ִ��
							Call_Send_Order = SENDING_MESSAGE;//�л����Ͷ���״̬
							GSM_Send(Phone_Num,USART1_RX_String);//���Ͷ���
							PhoneNum_Clean(Phone_Num);//���Phone_Num�����е�����
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
	
	if(Mode == SENDING_MESSAGE)//�������Ӣ�Ķ��ų��Ȳ�Ϊ11λ��7λ�������ú���
	{
		Mode = NONE;//ģʽ�ָ������ֵ�������ظ�ִ��
		Call_Send_Order = SENDING_MESSAGE;//�л����Ͷ���״̬
		GSM_Send(Phone_Num,USART1_RX_String);//���Ͷ���
	  PhoneNum_Clean(Phone_Num);//���Phone_Num�����е�����
	}
}

void PhoneNum_Clean(char *PhoneNum)
{
	uint8_t i;
	for(i=0;i<50;i++)
	{
		PhoneNum[i] = '\0';
	}
}
