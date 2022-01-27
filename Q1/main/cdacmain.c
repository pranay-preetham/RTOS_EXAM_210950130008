#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>
#include <freertos/queue.h>

QueueHandle_t queue;
TaskHandle_t xHandle_1;
TaskHandle_t xHandle_2;
TaskHandle_t xHandle_3;
TaskHandle_t xHandle_4;
TaskHandle_t xHandle_5;


void task_1(void *pvparameters)
{
    
    while(1)
    {
       
        printf(" TASK1  RUNNING: STACK SIZE %d\n",uxTaskPriorityGet(NULL));
        vTaskDelay(1000/ portTICK_PERIOD_MS);
    }
}

void task_2(void *pvparameters)
{
    
    while(1)
    {
        
        printf(" TASK2  RUNNING: STACK SIZE %d\n",uxTaskPriorityGet(NULL));
        vTaskDelay(2000/ portTICK_PERIOD_MS);
    }
}

void task_3(void *pvparameters)
{
   
    while(1)
    {
   
        printf(" TASK3  RUNNING: STACK SIZE %d\n",uxTaskPriorityGet(NULL));
        vTaskDelay(5000/ portTICK_PERIOD_MS);
    }
}

void task_4(void *pvparameters)
{
    int task_data=0;
    while(1)
    {
        task_data++;
        printf(" TASK  RUNNING: STACK SIZE %d\n",uxTaskGetStackHighWaterMark(NULL));
        xQueueSendFromISR(queue,&task_data,NULL);
        vTaskDelay(1000/ portTICK_PERIOD_MS);
    }
}
void task_5(void *pvparameters)
{
    int rece_data=0; 
    while(1)
    {
        printf(" TASK  RUNNING: STACK SIZE %d\n",uxTaskGetStackHighWaterMark(NULL));
        xQueueReceiveFromISR(queue,&rece_data,NULL);
        printf("RECEIVED DATA %d\n",rece_data);
        vTaskDelay(1000/ portTICK_PERIOD_MS);
    }
}
void app_main()
{
    queue=xQueueCreate(4,sizeof(int));
    BaseType_t result;
    
     result=xTaskCreate(task_1,"task_1",2048,NULL,5,&xHandle_1);
   
    result=xTaskCreate(task_2,"task_2",2048,NULL,6,&xHandle_2);

   
    result=xTaskCreate(task_3,"task_3",2048,NULL,7,&xHandle_3);

  
    result=xTaskCreate(task_4,"task_4",2048,NULL,8,&xHandle_4);

    if(result==pdPASS)
    {
        printf("task4 created\n");
    }
    result=xTaskCreate(task_5,"task_5",2048,NULL,9,&xHandle_5);

    if(result==pdPASS)
    {
        printf("task5 created\n");
    }
}
