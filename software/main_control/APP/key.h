#ifndef __KEY_H
#define __KEY_H	 
 
#include "includes.h"

#define KEY_PRESS               0        //�̰�
#define KEY_LOOSEN              1        //�ɿ�
#define KEY_DELAY_COUNT_LONG    100      //����ʱ��     ����1s     ��λ:10ms
//#define KEY_DELAY_DOUBLE_COUNT  20     //˫��ʱ���� С��200ms  ��λ:10ms



typedef enum
{
    KEY_CH1   = 0x00,
	  KEY_CH2   = 0x01,
	  KEY_CH3   = 0x02,
    KEY_CHN_MAX,
}KEY_CHN_E;

typedef enum
{
    KEY_STA1_Up        = (uint8_t)0x00,  //��������
    KEY_STA2_DownShake = (uint8_t)0x01,  //���¶���
    KEY_STA3_Down      = (uint8_t)0x02,  //��������
	  KEY_STA4_UpShake   = (uint8_t)0x03,  //���𶶶�
}KEY_STA_E;


typedef enum
{
    KEY_VAL_NOT                = 0x00,        //û����
    KEY_VAL_SHORT_CLICK        = 0x01,        //����
    KEY_VAL_DOUBLE_CLICK       = 0x02,        //˫��
    KEY_VAL_SINGLE_LONG_PRESS  = 0x03,        //���� 
    KEY_VAL_LONG_PRESS_LOOSEN  = 0x04,        //�����ɿ�
}KET_VAL_E;




typedef struct
{
    u8  key_sta[KEY_CHN_MAX];   			  //���水��״̬
    u8  key_val[KEY_CHN_MAX];					  //���水����ֵ
		u8  key_click[KEY_CHN_MAX];         //���水������->��ֹ����
    u8  key_longpress[KEY_CHN_MAX];     //���水������->��ֹ����
    u16 key_presstime[KEY_CHN_MAX];	    //���津��������ʱ��(�����Ϊ����)
	  /*����Ϊ˫���������*/
//	u8  key_doubleclick[KEY_CHN_MAX];	  //���水��˫��->��ֹ����
//  u8  key_clickflag[KEY_CHN_MAX];     //����˫����־
//	u8  key_doubletime[KEY_CHN_MAX];    //����˫����ʱ��(�����������Ϊ˫��)
}KEY_t;

/***********�ⲿ���ú�����ʼ*********/
extern KEY_t g_Key;					//ȫ�ֵİ����ṹ����������Կ���һ������
void 	 KEY_Init(void);			//������ʼ��
void 	 Key_ScanTask(void);	//������ѯ���״̬������
//void Key_ScanTask2(void);
/***********�ⲿ���ú�������*********/


				    
#endif
