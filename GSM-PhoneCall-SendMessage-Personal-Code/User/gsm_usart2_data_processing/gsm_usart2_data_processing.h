#ifndef __GSM_USART2_DATA_PROCESSING_H
#define	__GSM_USART2_DATA_PROCESSING_H

#include "stm32f10x.h"

uint8_t GSM_Init(void);
void GSM_Call(char *USART1_RX_String);
void GSM_Send(char *Phone_Num,char *Content_Message);
uint8_t SIM_Check(void);
uint8_t Signal_Check(void);
uint8_t GSM_Restart(void);

#endif /*__GSM_USART2_DATA_PROCESSING_H*/
