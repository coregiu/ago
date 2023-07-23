#include "LED.h"
#include "FreeRTOS.h"

/*
*********************************************************************************************************
*	�� �� ��: LED_Init
*	����˵��: LED��ʼ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/ 
void LED_Init(void)
{

	  GPIO_InitTypeDef  GPIO_InitStructure;
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_2;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	  GPIO_SetBits(GPIOC,GPIO_Pin_0);
	  GPIO_SetBits(GPIOC,GPIO_Pin_2);
		GPIO_SetBits(GPIOD,GPIO_Pin_3);
}












