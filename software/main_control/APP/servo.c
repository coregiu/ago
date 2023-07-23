#include "servo.h"
#include "usart.h"


/*
*********************************************************************************************************
*	�� �� ��: PWM_gpio����
*	����˵��: PWM�����������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/ 
void Servo_Gpio_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 	//ʹ��PORTDʱ��	
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;           //GPIOD15
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;         //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	 //�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;       //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;         //����
	GPIO_Init(GPIOD,&GPIO_InitStructure);                //��ʼ��PD15
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource15,GPIO_AF_TIM4); //GPIOD15����Ϊ��ʱ��4
	
}


/*
*********************************************************************************************************
*	�� �� ��: TIM4_Int_Init
*	����˵��: ��ʱ����ʼ������
*	��    ��: 1.�Զ���װֵ  2.ʱ��Ԥ��Ƶ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/ 
void TIM4_Int_Init(u16 reload ,u16 prescaler)
{
	Servo_Gpio_Config();
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  ///ʹ��TIM4ʱ��
	
	TIM_TimeBaseInitStructure.TIM_Period = reload; 	     //�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=prescaler;   //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);//��ʼ��TIM4
	
	//��ʼ��TIM4 Channel4 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	TIM_OCInitStructure.TIM_Pulse = 0;				//�Ƚϳ�ʼֵ
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM4 OC4
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM4��CCR4�ϵ�Ԥװ�ؼĴ���
 
	TIM_ARRPreloadConfig(TIM4,ENABLE);//ARPEʹ�� 	
	TIM_Cmd(TIM4,ENABLE); 	//ʹ�ܶ�ʱ��4
	
}


//���ö���Ƕ�
void Set_Servo_Angle(uint8_t angle)
{
	float T;
	uint32_t count;
	if(angle > 180)
	{
		return;
	}
	T = 0.5 + (angle*2/180);
	count = 999 * T / 20;
	TIM_SetCompare4(TIM4,count);
}

