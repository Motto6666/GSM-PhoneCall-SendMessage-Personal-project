#include "gsm_usart2_data_processing.h"
#include "bsp_gsm_usart2.h"
#include "bsp_systick.h"
#include "bsp_debug_usart1.h" //����ʹ�ã��������ɾ��

const char GSM_OK_Cmd[]  = {"OK"};//GSM���͵�OKָ��
extern volatile char  USART2_RX_String[50];//���USART2���ڽ��յ������ݵ�����
//static char Phone_Num_Str[50] = {"ATD18033156303;\r"};//��ŵ绰��������飬�������ɾ������

uint8_t GSM_Check_Response(void)//���GSMģ����Ӧ
{
	GSM_USART2_Send("AT+CGMM\r");//����GSM���������ɾ��
	SysTick_Delay_ms(2000);//��ʱ10ms���ȴ����ա�OK��ָ��
	printf("%s\n\n",USART2_RX_String);
	if( USART2_RX_String[0] == GSM_OK_Cmd[0] && USART2_RX_String[1] == GSM_OK_Cmd[1])
	{
		USART2_RX_Clean();//���USART2_RX_String[50]�����е��ڴ�
		return 1;
	}
	USART2_RX_Clean();//���USART2_RX_String[50]�����е��ڴ�
	return 0;
}

void GSM_Call_Cmd(void)
{
  if(GSM_Check_Response() == 1)
	{
		printf("GSMģ����Ӧ�ɹ�");
	}
	else
	{
		printf("GSMģ����Ӧʧ��");
	}
//  GSM_USART2_Send("ATD");//����GSM���������ɾ��
//	GSM_USART2_Send(USART1_RX_String);//����GSM���������ɾ��
//	GSM_USART2_Send(";\r");//����GSM���������ɾ��
}
