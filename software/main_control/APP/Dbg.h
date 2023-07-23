#ifndef __DBG_H__
#define __DBG_H__

#include "stm32f4xx.h"



/*
**********************************************************************************************************
											��ӡ��ؿ�ʼ
**********************************************************************************************************
*/

#define USER_DEBUG  		 //���ô�ӡ ->���ڴ������

#ifdef  USER_DEBUG
#define Println(format,...)   						\
{ 							  												\
	if(Print_Switch)												\
	{       				                        \
		printf(format "\r\n",##__VA_ARGS__); \
	}                                       \
}                                         
#else
    #define Println(format,...)          
#endif

#ifdef  USER_DEBUG
#define PrintlnErr(format,...)   					\
{ 							  												\
	if(Print_Switch)												\
	{       				                        \
		printf("[%-10s](%d) : " format "\r\n",__func__,__LINE__,##__VA_ARGS__); \
	}                                       \
}                                         
#else
    #define PrintlnErr(format,...)          
#endif
/*
**********************************************************************************************************
											��ӡ��ؽ���
**********************************************************************************************************
*/

extern u8 Print_Switch;


#endif

