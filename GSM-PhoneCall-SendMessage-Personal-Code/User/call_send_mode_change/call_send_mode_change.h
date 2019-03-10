#ifndef __CALL_SEND_MODE_CHANGE
#define __CALL_SEND_MODE_CHANGE

#include "stm32f10x.h"

/*拨打电话和发送短信标识符宏定义*/
#define  NONE                   0
#define  CALL_PHONE_PREPARE     1
#define  SEND_MESSAGE_PREPARE   2 
#define  ERROR                  3
#define  CALL_ALREADY_PREPARE   4
#define  SEND_ALREADY_PREPARE   5
#define  CALLING_PHONE          6
#define  CONTENT_MESSAGE        7
#define  SENDING_MESSAGE        8

void Call_Send_Mode_Change(char *USART1_RX_String);
void PhoneNum_Clean(char *PhoneNum);

#endif /*__CALL_SEND_MODE_CHANGE*/
