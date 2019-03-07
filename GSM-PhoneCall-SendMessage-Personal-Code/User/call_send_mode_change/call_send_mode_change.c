/*********************************
*�ı�GSMģ�鲦��绰�ͷ��Ͷ���ģʽ
**********************************/

#include "call_send_mode_change.h"
#include "string.h"
#include "bsp_debug_usart1.h"

const char CallPhone_String[50]   = {"CallPhone"};//����绰�ַ�����ʶ��
const char SendMessage_String[50] = {"SendMessage"};//���Ͷ����ַ�����ʶ��
volatile uint8_t Call_Send_Order = NONE;//����绰�ͷ��Ͷ��ű�ʶ������ʼֵΪ'0'
static char Phone_Num_Str[50];//��ŵ绰���������
volatile uint8_t Mode = NONE;//����绰�ͷ��Ͷ���ģʽ����ʼֵΪ0

void Call_Send_Mode_Change(char *USART1_RX_String)
{
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
							Mode = NONE;//ģʽ�ָ������ֵ�������ظ�ִ��
							Call_Send_Order = CALLING_PHONE;
							strcpy(Phone_Num_Str,USART1_RX_String);//�����յ��ĵ绰������뵽Phone_Num_Str������
						}break;
						
						case SEND_Already_Prepare:
						{
							Mode = NONE;//ģʽ�ָ������ֵ�������ظ�ִ��
							Call_Send_Order = SENDING_MESSAGE;//�л����Ͷ���״̬
							strcpy(Phone_Num_Str,USART1_RX_String);
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
}
