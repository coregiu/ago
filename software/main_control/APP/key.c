#include "Key.h"

/*����һ���������͵Ľṹ��*/
KEY_t g_Key = {0};



/*
*********************************************************************************************************
*	�� �� ��: KEY_Init
*	����˵��: ������ʼ�� ��ʼ��GPIOE_10,11,12Ϊ������������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/ 
void KEY_Init(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);//ʹ��PORTEʱ��
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12;//PE10,11,12
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN; //���ó�����
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP; //��������
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOE
} 


/*
*********************************************************************************************************
*	�� �� ��: Key_GetIoState
*	����˵��: ��������״̬��ȡ
*	��    ��: 1.�������
*	�� �� ֵ: ���ŵ�ƽ���ߵ�ƽΪ:1,�͵�ƽΪ:0��
*********************************************************************************************************
*/ 
u8 Key_GetIoState(u8 id)
{
    u8 val = 0;
    switch(id)
    {
        case KEY_CH1:
            val = GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_10);
            break;
				
        case KEY_CH2:
            val = GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_11);
            break;
				
        case KEY_CH3:
            val = GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_12);
            break;
    }
    return val;
}



/*
*********************************************************************************************************
*	�� �� ��: Key_ScanTask
*	����˵��: ������ѯ״̬��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/ 
void Key_ScanTask(void)
{
    static u32 update_10ms = 0;       	//��̬������������ʱ�����бȽϣ��ﵽ���ӳٵ�Ч��
    if(SysTick_1ms - update_10ms > 10)  //10ms����һ�Σ�Ҳ����������������
    {
        update_10ms = SysTick_1ms;   //����ʱ��
        for(u8 i=0; i<KEY_CHN_MAX; i++)
        {
            switch(g_Key.key_sta[i])
            {
                case KEY_STA1_Up:       //��������״̬
                    if( Key_GetIoState(i) == KEY_PRESS )   //��⵽�������µĵ�ƽ
                    {          
												g_Key.key_sta[i] = KEY_STA2_DownShake;   //������������״̬
                    }
                    else                                   //û�м�⵽��������
                    {
                        g_Key.key_sta[i] = KEY_STA1_Up;   //���ڵ�ǰ״̬
                        g_Key.key_val[i] = KEY_VAL_NOT;   //��ֵΪ�ް�������
                    }
                  break;
					
                case KEY_STA2_DownShake:      //��������״̬
                    if( Key_GetIoState(i) == KEY_PRESS )   //��⵽�������ǰ��µĵ�ƽ
                    {  
												g_Key.key_sta[i] = KEY_STA3_Down;  //��ת����������״̬
												g_Key.key_presstime[i] = 0;        //�������º�Ҫ���г�����⣬���Գ���ʱ������
										}	
										else
										{
												g_Key.key_sta[i] = KEY_STA1_Up;   //��ת����������״̬
										}
									break;
					
							  case KEY_STA3_Down:           //��������״̬
										g_Key.key_presstime[i]++;          //������ʱʱ��++;
										if(g_Key.key_presstime[i] < KEY_DELAY_COUNT_LONG)  //����ʱ��С��2��
										{
											if(Key_GetIoState(i) == KEY_LOOSEN)          //�����ɿ�
											{
												g_Key.key_val[i] = KEY_VAL_SHORT_CLICK;   //��ֵΪ��������	
												g_Key.key_click[i] = true;                //��������
												g_Key.key_sta[i] = KEY_STA4_UpShake;   	  //�������������𶶶�״̬													
											}
										}
										else           //����ʱ�����2�룬��Ϊ����
										{
											if(Key_GetIoState(i) == KEY_LOOSEN)              //�����ɿ�
											{
												 g_Key.key_val[i] = KEY_VAL_LONG_PRESS_LOOSEN; //��ֵΪ�����ɿ�
												 g_Key.key_sta[i] = KEY_STA4_UpShake;   	     //�������������𶶶�״̬
											}
											else 
											{
												 if(g_Key.key_val[i] != KEY_VAL_SINGLE_LONG_PRESS)
												 {
													 g_Key.key_longpress[i] = true;                //��������											 
												 }
												 g_Key.key_val[i] = KEY_VAL_SINGLE_LONG_PRESS; //��ֵΪ����״̬ 
											}					
										}								
                  break;
				
								case KEY_STA4_UpShake:		  //�������𶶶�
										if(Key_GetIoState(i) == KEY_LOOSEN)   //�������ɿ���
										{
											 g_Key.key_sta[i] = KEY_STA1_Up;   	       //��������������״̬
											 g_Key.key_val[i] = KEY_VAL_NOT;           //û�а�������״̬
										}
//										else   									//���ֻ�ǰ�������
//										{
//										    g_Key.key_sta[i] = KEY_STA3_Down;   	   //���ص���������״̬
//										}
								 break;
							default:  g_Key.key_sta[i] = KEY_STA1_Up;   	 //��������������״̬
            } 
        }
    }
}

//֧��˫��
//void Key_ScanTask1(void)
//{
//    static u32 update_10ms = 0; 
//    
//    if(SysTick_1ms - update_10ms > 10)
//    {
//        update_10ms = SysTick_1ms;   //����ʱ��
//        
//        for(u8 i=0; i<KEY_CHN_MAX; i++)
//        {
//            switch(g_Key.key_sta[i])
//            {
//                case KEY_STA1_Up:       //��������״̬
//                    if( Key_GetIoState(i) == KEY_PRESS )   //��⵽�������µĵ�ƽ
//                    {           
//											g_Key.key_sta[i] = KEY_STA2_DownShake;   //������������״̬
//                    }
//                    else                                   //û�м�⵽��������
//                    {
//											if(g_Key.key_clickflag[i] == 1)    //�е�����־
//											{
//												 g_Key.key_doubletime[i] ++;     //˫��ʱ����++
//												 if(g_Key.key_doubletime[i] >=  KEY_DELAY_DOUBLE_COUNT)  //����˫��ʱ����Ϊ����
//												 {
//													g_Key.key_clickflag[i] = 0;               //���������־
//													g_Key.key_val[i] = KEY_VAL_SHORT_CLICK;   //��ֵΪ����	
//													g_Key.key_click[i] = true;                //��������
//												 }
//												
//											}
//											else
//											{
//												g_Key.key_sta[i] = KEY_STA1_Up;   //���ڵ�ǰ״̬
//												g_Key.key_val[i] = KEY_VAL_NOT;   //��ֵΪ�ް�������						
//											}

//                    }
//					
//                    break;
//					
//                case KEY_STA2_DownShake:      //��������״̬
//                    if( Key_GetIoState(i) == KEY_PRESS )   //��⵽�������ǰ��µĵ�ƽ
//                    {  
//												g_Key.key_sta[i] = KEY_STA3_Down;  //��ת����������״̬
//												g_Key.key_presstime[i] = 0;        //�������º�Ҫ���г�����⣬���Գ���ʱ������
//										}	
//										else
//										{
//												g_Key.key_sta[i] = KEY_STA1_Up;   //��ת����������״̬
//										}
//                    break;
//                                    
//                case KEY_STA3_Down:           //��������״̬
//									g_Key.key_presstime[i]++;          //������ʱʱ��++;
//									if(g_Key.key_presstime[i] < KEY_DELAY_COUNT_LONG)  //����ʱ��С��2��
//									{
//										if(Key_GetIoState(i) == KEY_LOOSEN)           //�����ɿ�
//										{
//											if(g_Key.key_clickflag[i] == 1)            //�е����ı�־
//											{ 
//													g_Key.key_clickflag[i] = 0;               //������־���
//													g_Key.key_val[i] = KEY_VAL_DOUBLE_CLICK;  //��ֵΪ˫��
//														g_Key.key_doubleclick[KEY_CH1] = true;    //����˫��
//													g_Key.key_sta[i] = KEY_STA4_UpShake;   	  //�������������𶶶�״̬								
//											}
//											else
//											{
//												g_Key.key_clickflag[i] = 1;               //������־��һ
//												g_Key.key_doubletime[i] = 0;              //���˫��ʱ����
//												g_Key.key_sta[i] = KEY_STA4_UpShake;   	  //�������������𶶶�״̬								
//											}						
//										}
//									}
//									else           //����ʱ�����2�룬��Ϊ����
//									{
//										if(Key_GetIoState(i) == KEY_LOOSEN)              //�����ɿ�
//										{
//											 g_Key.key_val[i] = KEY_VAL_LONG_PRESS_LOOSEN; //��ֵΪ�����ɿ�
//											 g_Key.key_sta[i] = KEY_STA4_UpShake;   	     //�������������𶶶�״̬
//										}
//										else 
//										{ 
//											 if(g_Key.key_val[i] != KEY_VAL_SINGLE_LONG_PRESS)
//											 {
//												 g_Key.key_longpress[i] = true;
//											 }
//											 g_Key.key_val[i] = KEY_VAL_SINGLE_LONG_PRESS; //��ֵΪ����״̬
//											 
//											
//										}		
//									}			
//          break;
//				
//				case KEY_STA4_UpShake:		  //�������𶶶�
//						if(Key_GetIoState(i) == KEY_LOOSEN)   //�������ɿ���
//						{
//							 g_Key.key_sta[i] = KEY_STA1_Up;   	     //��������������״̬

//						}
//					
//					 break;
//				default:  g_Key.key_sta[i] = KEY_STA1_Up;   	 //��������������״̬
//        } 
//      }
//    }
//}


