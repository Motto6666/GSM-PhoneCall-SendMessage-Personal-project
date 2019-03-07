#include "stm32f10x.h"
#include "bsp_debug_usart1.h"
#include "call_send_mode_change.h"
#include "stdio.h"

extern volatile uint8_t Call_Send_Order;//����绰�ͷ��Ͷ��ű�ʶ������ʼֵΪ0
extern volatile uint8_t Mode;//����绰�ͷ��Ͷ���ģʽ����ʼֵΪ0

int main(void)
{	
  USART1_Config();//��ʼ��USART1����
	printf("\r\n��ʾ������CallPhoneΪ����绰������SendMessageΪ���Ͷ���\r\n");
	printf("\r\nע�⣺�ַ�������Ҫ��ӿո񣡣�\r\n");
  while(1)
	{
		switch(Call_Send_Order)
		{
			case CALL_PHONE_Prepare:
			{
				printf("\r\n������绰���룺\r\n");
				Mode = CALL_Already_Prepare;//�л�׼������绰ģʽ
				Call_Send_Order = NONE;//�����ظ�����
			}break;
			
			case CALLING_PHONE:
			{ 
				printf("\r\n���ڲ���������ĵ绰....\r\n");
				Call_Send_Order = NONE;//�����ظ�����
			}break;
			
			case SEND_MESSAGE_Prepare:
			{
				printf("\r\n��������ŵ绰���룺\r\n");
				Mode = SEND_Already_Prepare;//�л�׼�����Ͷ���ģʽ
				Call_Send_Order = NONE;//�����ظ�����
			}break;
			
			case SENDING_MESSAGE:
			{ 
				printf("\r\n���ڷ��Ͷ���....\r\n");
				Call_Send_Order = NONE;//�����ظ�����
			}break;
			
			case ERROR:
			{ 
				printf("\r\n����������Ϣ�����������룡\r\n");
				Call_Send_Order = NONE;//�����ظ�����
			}break;
			
			default: break;
		}
	}	
}
