/*********************************
*�ı�GSMģ�鲦��绰�ͷ��Ͷ���ģʽ
**********************************/

#include "call_send_mode_change.h"
#include "string.h"
#include "bsp_debug_usart1.h"
#include "bsp_gsm_usart2.h" //����USART2ͷ�ļ�������GSM���������ɾ�����
#include "gsm_usart2_data_processing.h"

const char CallPhone_String[50]   = {"CallPhone"};//����绰�ַ�����ʶ��
const char SendMessage_String[50] = {"SendMessage"};//���Ͷ����ַ�����ʶ��
volatile uint8_t Call_Send_Order = NONE;//����绰�ͷ��Ͷ��ű�ʶ������ʼֵΪ'0'
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
							GSM_Call_Cmd();
//							GSM_USART2_Send("AT");//����GSM���������ɾ��
//						  GSM_USART2_Send("ATD");//����GSM���������ɾ��
//							GSM_USART2_Send(USART1_RX_String);//����GSM���������ɾ��
//							GSM_USART2_Send(";\r");//����GSM���������ɾ��
							
						}break;
						
						case SEND_Already_Prepare:
						{
							Mode = NONE;//ģʽ�ָ������ֵ�������ظ�ִ��
							Call_Send_Order = SENDING_MESSAGE;//�л����Ͷ���״̬
							//strcpy(Phone_Num_Str,USART1_RX_String);
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
