#ifndef __USART_H
#define __USART_H

#include "includes.h"




/**********�ⲿ�������ÿ�ʼ*************/
void uart1_Init(u32 bound); 		//����1��ʼ������
void UartSendBuff(u8 * buff, u16 len);
void Printf(USART_TypeDef* USARTx, char* fmt, ...);
void uart1_process(void);
/**********�ⲿ�������ý���*************/


#endif
