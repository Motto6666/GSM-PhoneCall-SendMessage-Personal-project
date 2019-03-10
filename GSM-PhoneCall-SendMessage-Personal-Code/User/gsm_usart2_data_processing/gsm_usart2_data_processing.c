#include "gsm_usart2_data_processing.h"
#include "bsp_gsm_usart2.h"
#include "bsp_systick.h"
#include "bsp_debug_usart1.h" 
#include "string.h"

extern char  USART2_RX_String[50];//´æ·ÅUSART2´®¿Ú½ÓÊÕµ½µÄÊı¾İµÄÊı×é
//static char Phone_Num_Str[50] = {"ATD18033156303;\r"};//´æ·Åµç»°ºÅÂëµÄÊı×é£¬µ÷ÊÔÍê±ÏÉ¾³ı£¡£¡

/*GSM³õÊ¼»¯º¯Êı*/
uint8_t GSM_Init(void)
{
	SysTick_Delay_ms(1000);//µÈ´ıGSMÄ£¿é¿ª»ú
	GSM_USART2_Send("AT+CGMM\r");
	SysTick_Delay_ms(100);//µÈ´ıUSART2_RX½ÓÊÕÊı¾İ
//	printf("%s\n",USART2_RX_String);//µ÷ÊÔÍê±ÏÉ¾³ı£¡
//	printf("%c\n",USART2_RX_String[29]);//µ÷ÊÔÍê±ÏÉ¾³ı£
	//if( USART2_RX_String[28] == 'O' && USART2_RX_String[29] == 'K')//ÅĞ¶ÏÊÇ·ñ½ÓÊÕµ½GSM·¢ËÍ¹ıÀ´µÄ"OK"×Ö·
	if( strstr(USART2_RX_String,"OK") != NULL)//ÅĞ¶ÏÊÇ·ñ½ÓÊÕµ½GSM·¢ËÍ¹ıÀ´µÄ"OK"×Ö·û
	{
		printf("GSMÄ£¿éÏìÓ¦³É¹¦\n");
		USART2_RX_Clean();//Çå³ıUSART2_RX_String[50]Êı×éÖĞµÄÊı¾İ
		return 1;
	}
	else
	{
		printf("GSMÄ£¿éÏìÓ¦Ê§°Ü\n");
		USART2_RX_Clean();//Çå³ıUSART2_RX_String[50]Êı×éÖĞµÄÊı¾İ
	  return 0;
	}
}


/*GSMµç»°²¦´òº¯Êı*/
void GSM_Call(char *USART1_RX_String)//¼ì²éGSMÄ£¿éÏìÓ¦
{
	USART2_RX_Clean();//Çå³ıUSART2_RX_String[50]Êı×éÖĞµÄÊı¾İ
	//GSM_USART2_Send("ATD18033156303;\r");//ÏòGSMÄ£¿é·¢ËÍ²¦´òµç»°Ö¸Áî
	
  GSM_USART2_Send("ATD");//µ÷ÊÔGSM£¬µ÷ÊÔÍê±ÏÉ¾³ı
  GSM_USART2_Send(USART1_RX_String);//µ÷ÊÔGSM£¬µ÷ÊÔÍê±ÏÉ¾³ı
  GSM_USART2_Send(";\r");//µ÷ÊÔGSM£¬µ÷ÊÔÍê±ÏÉ¾³ı

//	while(strstr( USART2_RX_String,"NO ANSWER") == NULL );//ÅĞ¶ÏÊÇ·ñ½ÓÊÕµ½GSM·¢ËÍ¹ıÀ´µÄ"OK"×Ö·û
//	SysTick_Delay_ms(100);//µÈ´ıUSART2_RX½ÓÊÕÊı¾İ
//	printf("%s\n",USART2_RX_String);//µ÷ÊÔÍê±ÏÉ¾³ı£¡£¡
//	printf("%c\n",USART2_RX_String[0]);//µ÷ÊÔÍê±ÏÉ¾³ı£¡£¡
//	if( USART2_RX_String[0] == 'O')
//	{
//		printf("µç»°²¦´ò³É¹¦\n");
//		USART2_RX_Clean();//Çå³ıUSART2_RX_String[50]Êı×éÖĞµÄÊı¾İ
//	}
//	else
//	{
//		printf("µç»°²¦´òÊ§°Ü\n");
//		USART2_RX_Clean();//Çå³ıUSART2_RX_String[50]Êı×éÖĞµÄÊı¾İ
//	}
}


/*GSM¶ÌĞÅ·¢ËÍº¯Êı*/
char end = 0x1A;//·¢ËÍ¶ÌĞÅ½áÊø·ûºÅ
void GSM_Send(char *Phone_Num,char *Content_Message)
{
	char CmdBuff[50];//´æ·ÅÖ¸ÁîÊı×é
	
  USART2_RX_Clean();//Çå³ıUSART2_RX_String[50]Êı×éÖĞµÄÊı¾İ
			
  GSM_USART2_Send("AT+CSCS=\"GSM\"\r");
	SysTick_Delay_ms(100);
	while( strstr(USART2_RX_String,"OK") == NULL);//¼ì²âÊÇ·ñÓĞOKÊı¾İÖ¡·µ»Ø
	printf("%s\n",USART2_RX_String);//µ÷ÊÔÍê±ÏÉ¾³ı£¡!
	USART2_RX_Clean();//Çå³ıUSART2_RX_String[50]Êı×éÖĞµÄÊı¾İ
	
	GSM_USART2_Send("AT+CMGF=1\r");
	SysTick_Delay_ms(100);
	while( strstr(USART2_RX_String,"OK") == NULL);//¼ì²âÊÇ·ñÓĞOKÊı¾İÖ¡·µ»Ø
	printf("%s\n",USART2_RX_String);//µ÷ÊÔÍê±ÏÉ¾³ı£¡!
	USART2_RX_Clean();//Çå³ıUSART2_RX_String[50]Êı×éÖĞµÄÊı¾İ
	
	sprintf( CmdBuff,"AT+CMGS=\"%s\"\r",Phone_Num);
	GSM_USART2_Send(CmdBuff);
	SysTick_Delay_ms(200);//µÈ´ıUSART2_RX½ÓÊÕÊı¾İ
	while( strstr(USART2_RX_String,">") == NULL);//¼ì²âÊÇ·ñÓĞ>Êı¾İÖ¡·µ»Ø
	printf("%s\n",USART2_RX_String);//µ÷ÊÔÍê±ÏÉ¾³ı£¡!
	USART2_RX_Clean();//Çå³ıUSART2_RX_String[50]Êı×éÖĞµÄÊı¾İ
	
	GSM_USART2_Send(Content_Message);//·¢ËÍ¶ÌĞÅÀàÈİ
	
	GSM_USART2_Send( &end );//·¢ËÍ½áÊøÖ¸Áî
	SysTick_Delay_ms(5000);//µÈ´ıUSART2_RX½ÓÊÕÊı¾İ
	while( strstr(USART2_RX_String,"OK") == NULL);//¼ì²âÊÇ·ñÓĞOKÊı¾İÖ¡·µ»Ø
	printf("%s\n",USART2_RX_String);//µ÷ÊÔÍê±ÏÉ¾³ı£¡!
	USART2_RX_Clean();//Çå³ıUSART2_RX_String[50]Êı×éÖĞµÄÊı¾İ
}


