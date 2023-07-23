#include "usart.h"
#include "stdarg.h"
#include "String.h"
#include "ringbuff.h"

#define DATA_RX_BUFSIZE 256

static ringbuff_t s_recieve_data_ring_buf;
static uint8_t s_rx_buf[DATA_RX_BUFSIZE];

/*
*********************************************************************************************************
*	�� �� ��: ring_buffer_init
*	����˵��: ��ʼ�����λ�����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/ 
void ring_buffer_init(void)
{
  ringbuff_init(&s_recieve_data_ring_buf,s_rx_buf,DATA_RX_BUFSIZE);
}

/*
*********************************************************************************************************
*	�� �� ��: uart1_NvicConfig
*	����˵��: ����1NVIC����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/ 
void uart1_NvicConfig(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;       //����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3; //��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		    //����4���������ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			    //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
	
}


/*
*********************************************************************************************************
*	�� �� ��: uart1_Init
*	����˵��: ����1��ʼ������
*	��    ��: 1.bound�����ڲ����ʣ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/ 
void uart1_Init(u32 bound)
{
   //GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10����ΪUSART1
	
	//USART1�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOA,&GPIO_InitStructure); //��ʼ��PA9��PA10

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode =  USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
	uart1_NvicConfig();  //NVIC����
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  //�������ڽ����ж�
//	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);  //�������ڿ����ж�
	
	USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1 
	//USART_ClearFlag(USART1, USART_FLAG_TC);   //�����һ���ֽڶ�ʧ����,�����һ���ֽڶ�ʧ���Լ�������
	ring_buffer_init();    //��ʼ�����λ�����
}


//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;  

//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 

/*
*********************************************************************************************************
*	�� �� ��: fputc
*	����˵��: �ض���fputc����
*	��    ��: 1:ch(��д���ַ�) 2:f(�ļ�ָ��)
*	�� �� ֵ: ��
*********************************************************************************************************
*/ 
#define USE_REGISTER      //�Ƿ�ʹ�üĴ�������

int fputc(int ch, FILE *f)
{ 	

#ifdef 	USE_REGISTER      //ʹ�üĴ�������

	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;  
	
#else	                    //ʹ�ÿ⺯������
	
	USART_SendData(USART1, (unsigned char)ch);
    while (!USART_GetFlagStatus(USART1, USART_FLAG_TC));
    USART_ClearFlag(USART1, USART_FLAG_TC);
	
#endif
	
	return ch;
}


/*
*********************************************************************************************************
*	�� �� ��: Printf
*	����˵��: �ɱ�����ض���Printf����
*	��    ��: 1.���ں� 2.char* fmt,...  ��ʽ������ַ����Ͳ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/ 
#define    USART_SENDBUFFLEN     256          //�����ӡ���ַ������鳤��

char g_USART_SendBuff[USART_SENDBUFFLEN];     //���ڴ�ӡ���ַ�������

void Printf(USART_TypeDef* USARTx, char* fmt, ...)
{
   
		u16 i = 0;                                //����±�
		memset(g_USART_SendBuff,0,USART_SENDBUFFLEN); //����ַ�����
	
		va_list ap;                               //�ɱ�����б����������
		va_start(ap,fmt);                         //ָ��char*���͵�fmt��������Ĳ��ǿɱ�����Ŀ�ʼ
		vsprintf(g_USART_SendBuff,fmt,ap);        //ʹ�ò����б��͸�ʽ��������ַ���
		va_end(ap);	                              //��ղ����б����������

	    while(g_USART_SendBuff[i])                //����ַ�������ΪNULL���˳���ӡ
		{
		  while((USARTx->SR&0X40)==0);
		  USARTx->DR = g_USART_SendBuff[i++];
		}
}


/*
*********************************************************************************************************
*	�� �� ��: UartSendBuff
*	����˵��: ����1���ͺ���
*	��    ��: Ҫ���͵�����  ��  ���ݳ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void UartSendBuff(u8 * buff, u16 len)
{
	while(len--)
	{
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET); //�ȴ����ͽ���
		USART_SendData(USART1, *buff++);
	}
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
}

/*
*********************************************************************************************************
*	�� �� ��: USART1_IRQHandler
*	����˵��: ����1�����жϷ�����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/ 

void USART1_IRQHandler(void)  //����1�жϷ������
{
	uint8_t receive_data;
	
	if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET) //�����жϴ���
	{		
		receive_data = USART_ReceiveData(USART1); 
		//USART_SendData(USART1,receive_data);
		//д�뻷�λ�����
		ringbuff_write(&s_recieve_data_ring_buf,&receive_data,1);
	}	
}

/*
*********************************************************************************************************
*	�� �� ��: uart1_process
*	����˵��: ���ݴ�����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/ 
void uart1_process(void)
{
	uint8_t buf[DATA_RX_BUFSIZE] = {0};
	uint32_t datalen = 0;
	datalen = ringbuff_get_linear_block_length(&s_recieve_data_ring_buf);
	if(datalen != 0)
	{
		ringbuff_read(&s_recieve_data_ring_buf,buf,datalen);
		UartSendBuff(buf,datalen);
	}
}

