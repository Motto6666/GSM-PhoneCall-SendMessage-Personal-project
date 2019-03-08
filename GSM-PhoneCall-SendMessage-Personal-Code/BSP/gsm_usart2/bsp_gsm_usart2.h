#ifndef __BSP_GSM_UARST2_H
#define	__BSP_GSM_UARST2_H


#include "stm32f10x.h"
	
/*串口2-USART2*/
#define  GSM_USART2                    USART2
#define  GSM_USART2_CLK                RCC_APB1Periph_USART2
#define  GSM_USART2_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  GSM_USART2_BAUDRATE           115200

/*USART2 GPIO 引脚宏定义*/
#define  GSM_USART2_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  GSM_USART2_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  GSM_USART2_TX_GPIO_PORT         GPIOA   
#define  GSM_USART2_TX_GPIO_PIN          GPIO_Pin_2
#define  GSM_USART2_RX_GPIO_PORT         GPIOA
#define  GSM_USART2_RX_GPIO_PIN          GPIO_Pin_3


/*USART2 中断服务函数宏定义*/
#define  GSM_USART2_IRQ                USART2_IRQn
#define  GSM_USART2_IRQHandler         USART2_IRQHandler


void GSM_USART2_Config(void);
void GSM_USART2_Send(char * str);
void USART2_RX_Clean(void);

#endif /*__BSP_GSM_UARST2_H*/
