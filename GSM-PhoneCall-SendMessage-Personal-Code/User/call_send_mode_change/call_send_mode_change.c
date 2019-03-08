/*********************************
*¸Ä±äGSMÄ£¿é²¦´òµç»°ºÍ·¢ËÍ¶ÌĞÅÄ£Ê½
**********************************/

#include "call_send_mode_change.h"
#include "string.h"
#include "bsp_debug_usart1.h"
#include "bsp_gsm_usart2.h" //Áô×ÅUSART2Í·ÎÄ¼ş£¬µ÷ÊÔGSM£¬µ÷ÊÔÍê±ÏÉ¾³ı£¡£
#include "gsm_usart2_data_processing.h"

const char CallPhone_String[50]   = {"CallPhone"};//²¦´òµç»°×Ö·û´®±êÊ¶·û
const char SendMessage_String[50] = {"SendMessage"};//·¢ËÍ¶ÌĞÅ×Ö·û´®±êÊ¶·û
volatile uint8_t Call_Send_Order = NONE;//²¦´òµç»°ºÍ·¢ËÍ¶ÌĞÅ±êÊ¶·û£¬³õÊ¼ÖµÎª'0'
volatile uint8_t Mode = NONE;//²¦´òµç»°ºÍ·¢ËÍ¶ÌĞÅÄ£Ê½£¬³õÊ¼ÖµÎª0

void Call_Send_Mode_Change(char *USART1_RX_String)
{
	if(strcmp(CallPhone_String,USART1_RX_String) == 0)//ÅĞ¶ÏUSART1´®¿Ú½ÓÊÕµ½µÄÊı¾İÊÇ·ñÎª²¦´òµç»°×Ö·û´®±êÊ¶·û
	{
		Call_Send_Order = CALL_PHONE_Prepare;
	}
			
	if(strcmp(SendMessage_String,USART1_RX_String) == 0)//ÅĞ¶ÏUSART1´®¿Ú½ÓÊÕµ½µÄÊı¾İÊÇ·ñÎª·¢ËÍ¶ÌĞÅ×Ö·û´®±êÊ¶·û
	{
		Call_Send_Order = SEND_MESSAGE_Prepare;
	}
			
	if((strcmp(SendMessage_String,USART1_RX_String) != 0 )&& (strcmp(CallPhone_String,USART1_RX_String) != 0))
	{
				if(strlen(USART1_RX_String) == 11)//ÅĞ¶ÏÊäÈëµÄµç»°ºÅÂëÊÇ·ñÎª11Î»
				{
					switch(Mode)
					{
						case CALL_Already_Prepare:
						{
							Mode = NONE;//Ä£Ê½»Ö¸´µ½×î³õÖµ£¬±ÜÃâÖØ¸´Ö´ĞĞ
							Call_Send_Order = CALLING_PHONE;
							GSM_Call_Cmd();
//							GSM_USART2_Send("AT");//µ÷ÊÔGSM£¬µ÷ÊÔÍê±ÏÉ¾³ı
//						  GSM_USART2_Send("ATD");//µ÷ÊÔGSM£¬µ÷ÊÔÍê±ÏÉ¾³ı
//							GSM_USART2_Send(USART1_RX_String);//µ÷ÊÔGSM£¬µ÷ÊÔÍê±ÏÉ¾³ı
//							GSM_USART2_Send(";\r");//µ÷ÊÔGSM£¬µ÷ÊÔÍê±ÏÉ¾³ı
							
						}break;
						
						case SEND_Already_Prepare:
						{
							Mode = NONE;//Ä£Ê½»Ö¸´µ½×î³õÖµ£¬±ÜÃâÖØ¸´Ö´ĞĞ
							Call_Send_Order = SENDING_MESSAGE;//ÇĞ»»·¢ËÍ¶ÌĞÅ×´Ì¬
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
