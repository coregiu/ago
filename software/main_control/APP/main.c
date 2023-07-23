#include "includes.h"
#include "FreeRTOS.h"
#include "task.h"


void vTaskLed(void * pvParameters)
{
    while(1)
    {
       LED1_ON;
       vTaskDelay(1000);
       LED1_OFF;
       vTaskDelay(1000);
    }
}

int main(void)
{
	/* LED��ʼ�� */
	LED_Init();
	/* �������� */
	xTaskCreate(vTaskLed,"Task Led",512,NULL,1,NULL);
	/* �������ȣ� ��ʼִ������ */
	vTaskStartScheduler();
	while(1);
}
