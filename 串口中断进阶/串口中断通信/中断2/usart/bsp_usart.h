#ifndef __BSP_USART_H
#define __BSP_USART_H

#include "stm32f10x.h"
#include "string.h"

//串口1-USART1
#define  DEBUG_USARTx                   USART1
#define  DEBUG_USART_CLK                RCC_APB2Periph_USART1
#define  DEBUG_USART_APBxClkCmd         RCC_APB2PeriphClockCmd
#define  DEBUG_USART_BAUDRATE           115200

//USART GPIO 引脚宏定义
#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  DEBUG_USART_TX_GPIO_PORT       GPIOA   
#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_9
#define  DEBUG_USART_RX_GPIO_PORT       GPIOA
#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_10

#define  DEBUG_USART_IRQ                USART1_IRQn
#define  DEBUG_USART_IRQHandler         USART1_IRQHandler

//初始化串口
void USART_Config(void);
//发送一个字节
void Usart_SendByte(USART_TypeDef* pUSARTx,uint8_t data);
//发送字符串
void Usart_Sendstr(USART_TypeDef* pUSARTx,char *str);
//
void delay_ms(uint16_t delay_ms);

#endif /*__BSP_USART_H */
