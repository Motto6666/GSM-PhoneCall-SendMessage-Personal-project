#include "stm32f10x.h"
#include "bsp_debug_usart1.h"
#include "call_send_mode_change.h"
#include "stdio.h"
#include "bsp_gsm_usart2.h"
#include "gsm_usart2_data_processing.h"
#include "bsp_basetime.h"


extern volatile uint8_t Call_Send_Order;//����绰�ͷ��Ͷ��ű�ʶ������ʼֵΪ0
extern volatile uint8_t Mode;//����绰�ͷ��Ͷ���ģʽ����ʼֵΪ0
extern char  USART1_RX_String[50];//���USART1���ڽ��յ������ݵ�����
extern volatile uint8_t  Receive_Over;//�������ݽ�����ʶ������ʼ��Ϊ0
extern volatile uint8_t  GSM_SysCheck;//GSMϵͳ����־������ʼ��Ϊ0


int main(void)
{	
  USART1_Config();//��ʼ��USART1����
	GSM_USART2_Config();//��ʼ��USART2����
	
	while(!GSM_Init());//��ʼ��GSMģ��
		
	printf("\r\n��ʾ������CallPhoneΪ����绰������SendMessageΪ���Ͷ���\r\n");
	printf("\r\nע�⣺�ַ�������Ҫ���'+'���ţ���\r\n");

	BASIC_TIM_Init();//��ʼ��������ʱ��
	
  while(1)
	{
		switch(Call_Send_Order)
		{
			case CALL_PHONE_PREPARE:
			{
				printf("\r\n������绰���룺\r\n");
				Mode = CALL_ALREADY_PREPARE;//�л�׼������绰ģʽ
				Call_Send_Order = NONE;//�����ظ�����
			}break;
			
			case CALLING_PHONE:
			{ 
				printf("\r\n���ڲ���������ĵ绰....\r\n");
				Call_Send_Order = NONE;//�����ظ�����
			}break;
			
			case SEND_MESSAGE_PREPARE:
			{
				printf("\r\n��������ŵ绰���룺\r\n");
				Mode = SEND_ALREADY_PREPARE;//�л�׼�����Ͷ���ģʽ
				Call_Send_Order = NONE;//�����ظ�����
			}break;
			
			case CONTENT_MESSAGE:
			{
				printf("\r\n������Ӣ�Ķ������ݣ�\r\n");
				Mode = SENDING_MESSAGE;//�л�Ϊ���Ͷ���ģʽ
				Call_Send_Order = NONE;//�����ظ�����
			}break;
			
			case SENDING_MESSAGE:
			{ 
				printf("\r\n���Ͷ��Ŷ������\r\n");
				Call_Send_Order = NONE;//�����ظ�����
			}break;
			
			case ERROR:
			{ 
				printf("\r\n����������Ϣ�����������룡\r\n");
				Call_Send_Order = NONE;//�����ظ�����
			}break;
			
			default: break;
		}
		
		
		if(Receive_Over == 1)//USART1���ڽ������
		{
			Receive_Over = 0;//�������ݽ�����ʶ���ָ������ֵ�������ظ�����
			Call_Send_Mode_Change(USART1_RX_String);//�ж�������ַ�������GSM���ܵ��л�
			USART1_RX_Clean();//���USAET1���ڽ����ַ������棬�����USART1_RX_String[50]�е�����
		}
		
		
		if(GSM_SysCheck == 1)//ÿ2minִ��һ��GSMϵͳ���
		{
			GSM_SysCheck = 0;//GSMϵͳ����־���ָ������ֵ�������ظ�����
			printf("ִ��GSMϵͳ���.....\n");
			if(SIM_Check() == 0 || Signal_Check() == 0)//���GSM��SIM���͹����ź��Ƿ��������
			{
				GSM_Restart();//����GSMģ��
			}
			else
			{
				printf("GSMģ�鹤������\n");
			}	
		}
		
	}	
}
