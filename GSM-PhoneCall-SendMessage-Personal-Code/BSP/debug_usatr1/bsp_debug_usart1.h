#ifndef __BSP_DEBUG_UARST1_H
#define	__BSP_DEBUG_UARST1_H


#include "stm32f10x.h"
#include <stdio.h>
	
/*USART1*/
#define  DEBUG_USART1                   USART1
#define  DEBUG_USART_CLK                RCC_APB2Periph_USART1
#define  DEBUG_USART_APBxClkCmd         RCC_APB2PeriphClockCmd
#define  DEBUG_USART_BAUDRATE           115200

/*USART1 GPIO 引脚宏定义*/
#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  DEBUG_USART_TX_GPIO_PORT       GPIOA   
#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_9
#define  DEBUG_USART_RX_GPIO_PORT       GPIOA
#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_10


/*USART1 中断服务函数宏定义*/
#define  DEBUG_USART_IRQ                USART1_IRQn
#define  DEBUG_USART1_IRQHandler        USART1_IRQHandler


void USART1_Config(void);
void USART1_RX_Clean(void);


#endif /*__BSP_DEBUG_UARST1_H*/
