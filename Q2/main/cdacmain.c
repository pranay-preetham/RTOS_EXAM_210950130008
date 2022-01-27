#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>


TaskHandle_t xHandle_1;
TaskHandle_t xHandle_2;
TaskHandle_t xHandle_3;

void task_1(void *pvparameters)
{
    while(1)
    {
        printf(" TASK1 PRIORITY %d\n",uxTaskPriorityGet(NULL));
        vTaskDelay(1000/ portTICK_PERIOD_MS);
    }
}
void task_2(void *pvparameters)
{
   
    while(1)
    {
        printf(" TASK2 PRIORITY %d\n",uxTaskPriorityGet(NULL));
        
        vTaskDelay(2000/ portTICK_PERIOD_MS);
    }
}

void task_3(void *pvparameters)
{
   
    while(1)
    {
        printf(" TASK3 PRIORITY %d\n",uxTaskPriorityGet(NULL));

        vTaskDelay(5000/ portTICK_PERIOD_MS);
    }
}

void ISR(void *pv)
{
	int aux=0;
	xSemaphoreGiveFromISR(smf,&aux);
	if(aux)
	{
		portYIELD_FROM_ISR();
	}
}
void vBacklightTimerCallback(TimerHandle_t pxtimer)
{
	vSetBackLightState(BACKLIGHT_OFF);
}	
void vKeyPressEventInterruptHandler( void *pv )
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
 	vSetBacklightState( BACKLIGHT_ON );
 	if( xTimerStartFromISR( xBacklightTimer,&xHigherPriorityTaskWoken ) != pdPASS )
    	{
    		printf("one-shot timer\n");
   	}
   	if( xHigherPriorityTaskWoken != pdFALSE )
    	{
    		portYIELD_FROM_ISR();
    	}
}


void app_main()
{
     
	smf=xSemaphoreCreateBinary();
	tmr=xTimerCreate("tmr_smf",pdMS_TO_TICK(100),true,0,ISR);
	xTimerStart(tmr,pdMS_TO_TICKS(100));
	while(1)
	{
		if(xSemaphoreTake(smf,portMAX_DELAY))
		{
			printf("Software timers\n");
			count =! count;
		}
		vTaskDelay(pdMS_TO_TICKS(10));
	}

     
    BaseType_t result;
    
    result=xTaskCreate(task_1,"task_1",2048,NULL,5,&xHandle_1);
    result=xTaskCreate(task_2,"task_2",2048,NULL,6,&xHandle_2);
    result=xTaskCreate(task_3,"task_3",2048,NULL,7,&xHandle_3);
    if(result==pdPASS)
    {
        printf("tasks created\n");
    }
}
