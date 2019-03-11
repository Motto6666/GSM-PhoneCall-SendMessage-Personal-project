#include "stm32f10x.h"
#include "bsp_debug_usart1.h"
#include "call_send_mode_change.h"
#include "stdio.h"
#include "bsp_gsm_usart2.h"
#include "gsm_usart2_data_processing.h"

//#include "bsp_systick.h"//µ÷ÊÔÍê±ÏÉ¾³ı£¡!
//#include "string.h"//µ÷ÊÔÍê±ÏÉ¾³ı£¡!
//extern char  USART2_RX_String[50];//µ÷ÊÔÍê±ÏÉ¾³ı£¡!
//char end = 0x1A;//µ÷ÊÔÍê±ÏÉ¾³ı£¡!

extern volatile uint8_t Call_Send_Order;//²¦´òµç»°ºÍ·¢ËÍ¶ÌĞÅ±êÊ¶·û£¬³õÊ¼ÖµÎª0
extern volatile uint8_t Mode;//²¦´òµç»°ºÍ·¢ËÍ¶ÌĞÅÄ£Ê½£¬³õÊ¼ÖµÎª0
extern char  USART1_RX_String[50];//´æ·ÅUSART1´®¿Ú½ÓÊÕµ½µÄÊı¾İµÄÊı×é
extern volatile uint8_t  Receive_Over;//½ÓÊÕÊı¾İ½áÊø±êÊ¶·û£¬³õÊ¼»¯Îª0


int main(void)
{	
  USART1_Config();//³õÊ¼»¯USART1´®¿Ú
	GSM_USART2_Config();//³õÊ¼»¯USART2´®¿Ú
	
	while(!GSM_Init());//³õÊ¼»¯GSMÄ£¿é
		
	printf("\r\nÌáÊ¾£ºÊäÈëCallPhoneÎª²¦´òµç»°£¬ÊäÈëSendMessageÎª·¢ËÍ¶ÌĞÅ\r\n");
	printf("\r\n×¢Òâ£º×Ö·û´®ºóĞèÒªÌí¼Ó'+'·ûºÅ£¡£¡\r\n");

//	/*¼ì²âSIM¿¨²åÈëÊÇ·ñÒì³£*/
//	USART2_RX_Clean();//Çå³ıUSART2_RX_String[50]Êı×éÖĞµÄÊı¾İ
//  GSM_USART2_Send("AT+CNUM\r");//µ÷ÊÔGSM£¬µ÷ÊÔÍê±ÏÉ¾³ı
//	SysTick_Delay_ms(100);
//  printf("%s\n",USART2_RX_String);//µ÷ÊÔÍê±ÏÉ¾³ı£¡!
//	USART2_RX_Clean();//Çå³ıUSART2_RX_String[50]Êı×éÖĞµÄÊı¾İ
//	¡
//	/*¼ì²âĞÅºÅ*/
//	USART2_RX_Clean();//Çå³ıUSART2_RX_String[50]Êı×éÖĞµÄÊı¾İ
//  GSM_USART2_Send("AT+CSQ\r");//µ÷ÊÔGSM£¬µ÷ÊÔÍê±ÏÉ¾³ı
//	SysTick_Delay_ms(100);
//  printf("%s\n",USART2_RX_String);//µ÷ÊÔÍê±ÏÉ¾³ı£¡!
//	USART2_RX_Clean();//Çå³ıUSART2_RX_String[50]Êı×éÖĞµÄÊı¾İ
	
	
	
  while(1)
	{
		switch(Call_Send_Order)
		{
			case CALL_PHONE_PREPARE:
			{
				printf("\r\nÇëÊäÈëµç»°ºÅÂë£º\r\n");
				Mode = CALL_ALREADY_PREPARE;//ÇĞ»»×¼±¸²¦´òµç»°Ä£Ê½
				Call_Send_Order = NONE;//±ÜÃâÖØ¸´²Ù×÷
			}break;
			
			case CALLING_PHONE:
			{ 
				printf("\r\nÕıÔÚ²¦´òÄúÊäÈëµÄµç»°....\r\n");
				Call_Send_Order = NONE;//±ÜÃâÖØ¸´²Ù×÷
			}break;
			
			case SEND_MESSAGE_PREPARE:
			{
				printf("\r\nÇëÊäÈë¶ÌĞÅµç»°ºÅÂë£º\r\n");
				Mode = SEND_ALREADY_PREPARE;//ÇĞ»»×¼±¸·¢ËÍ¶ÌĞÅÄ£Ê½
				Call_Send_Order = NONE;//±ÜÃâÖØ¸´²Ù×÷
			}break;
			
			case CONTENT_MESSAGE:
			{
				printf("\r\nÇëÊäÈëÓ¢ÎÄ¶ÌĞÅÄÚÈİ£º\r\n");
				Mode = SENDING_MESSAGE;//ÇĞ»»Îª·¢ËÍ¶ÌĞÅÄ£Ê½
				Call_Send_Order = NONE;//±ÜÃâÖØ¸´²Ù×÷
			}break;
			
			case SENDING_MESSAGE:
			{ 
				printf("\r\n·¢ËÍ¶ÌĞÅ¶ÌĞÅÍê±Ï\r\n");
				Call_Send_Order = NONE;//±ÜÃâÖØ¸´²Ù×÷
			}break;
			
			case ERROR:
			{ 
				printf("\r\nÊäÈë´íÎóµÄĞÅÏ¢£¬ÇëÖØĞÂÊäÈë£¡\r\n");
				Call_Send_Order = NONE;//±ÜÃâÖØ¸´²Ù×÷
			}break;
			
			default: break;
		}
		
		if(Receive_Over == 1)
		{
			Receive_Over = 0;//½ÓÊÕÊı¾İ½áÊø±êÊ¶·û»Ö¸´µ½×î³õÖµ£¬±ÜÃâÖØ¸´²Ù×÷
			Call_Send_Mode_Change(USART1_RX_String);//ÅĞ¶ÏÊäÈëµÄ×Ö·û£¬×÷³öGSM¹¦ÄÜµÄÇĞ»»
			USART1_RX_Clean();//Çå³ıUSAET1´®¿Ú½ÓÊÕ×Ö·û´®»º´æ£¬¼´Çå¿ÕUSART1_RX_String[50]ÖĞµÄÊı¾İ
		}
	}	
}
