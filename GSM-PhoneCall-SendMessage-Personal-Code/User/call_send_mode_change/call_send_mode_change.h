#ifndef __CALL_SEND_MODE_CHANGE
#define __CALL_SEND_MODE_CHANGE

#include "stm32f10x.h"

/*拨打电话和发送短信标识符宏定义*/
#define  NONE                   0
#define  CALL_PHONE_Prepare     1
#define  SEND_MESSAGE_Prepare   2 
#define  ERROR                  3
#define  CALL_Already_Prepare   4
#define  SEND_Already_Prepare   5
#define  CALLING_PHONE          6
#define  SENDING_MESSAGE        7

void Call_Send_Mode_Change(char *USART1_RX_String);

#endif /*__CALL_SEND_MODE_CHANGE*/
