/******************** (C) COPYRIGHT 2013 www.armjishu.com  ***********************
 * �ļ���  ��main.c
 * ����    ��ʵ��STM32F107VC����IV�ſ�������ʵ�ִ��������������
 * ʵ��ƽ̨��STM32���ۿ�����
 * ��׼��  ��STM32F10x_StdPeriph_Driver V3.5.0
 * ����    ��www.armjishu.com 
**********************************************************************************/
#include "usart.h"
#include "stm32f10x.h"
#include "led.h"
#include "sht30.h"
#include "delay.h"
/*************FreeRTOSͷ�ļ�************************/
#include "FreeRTOS.h"
#include "task.h"

#include "modbus_poll.h"
float  Tem_Value_1;
float  RH_Value_1;


#define START_TASK_PRIO  1 //�������ȼ�
#define START_STK_SIZE   128 //�����ջ��С
TaskHandle_t  StartTask_Handler ;
void start_task(void *pvParameters);

//�������ȼ�
#define LED0_TASK_PRIO		3
//�����ջ��С	
#define LED0_STK_SIZE 		128 
//������
TaskHandle_t LED0Task_Handler;
//������
void led0_task(void *pvParameters);

//�������ȼ�
#define sht30_TASK_PRIO		2
//�����ջ��С	
#define sht30_STK_SIZE 		128  
//������
TaskHandle_t sht30Task_Handler;
//������
void sht30_task(void *pvParameters);

//�������ȼ�
#define LED1_TASK_PRIO		3
//�����ջ��С	
#define LED1_STK_SIZE 		128 
//������
TaskHandle_t LED1Task_Handler;
//������
void led1_task(void *pvParameters);

void vTaskFunction( void *pvParameters );

//static const char *pcTextForTask1 = "Task 1 is running\r\n";
//static const char *pcTextForTask2 = "Task 2 is running\r\n";

/**-------------------------------------------------------
  * @������ main
  * @����   ������
  * @����   ��
  * @����ֵ ��
***------------------------------------------------------*/


int main(void)
{ 
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);	  
	SZ_STM32_COMInit();	 /* ���ڳ�ʼ�� */
	delay_init();
  LED_Init();
	SHT_Init();
//	System_Setup(); 
	printf("\n\r WWW.ARMJISHU.COM! \n");

	//������ʼ����
    xTaskCreate((TaskFunction_t )start_task,            //������
                (const char*    )"start_task",          //��������
                (uint16_t       )START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   //������        
	  
    vTaskStartScheduler();          //�����������
	return 0;
}

//��ʼ����������
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //�����ٽ���
    //����LED0����
    xTaskCreate((TaskFunction_t )led0_task,     	
                (const char*    )"led0_task",   	
                (uint16_t       )LED0_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )LED0_TASK_PRIO,	
                (TaskHandle_t*  )&LED0Task_Handler);   
    //����LED1����
    xTaskCreate((TaskFunction_t )led1_task,     
                (const char*    )"led1_task",   
                (uint16_t       )LED1_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LED1_TASK_PRIO,
                (TaskHandle_t*  )&LED1Task_Handler);     
								
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}

//LED0������ 
void led0_task(void *pvParameters)
{
	uint8_t i ;
	uint16_t reg[10];
  mb_poll_status_t status;
  modbus_poll_init();
  modbus_poll_pre_transmission(0);
  modbus_poll_post_transmission(0);
  while(1)
  {
		/*������0x01 ��ȡ��Ȧ״̬*/
//    status= modbus_poll_readCoils(1,0,17,reg);
//    if(status!=MODBUS_POLL_STATUS_SUCCESS)
//		{
//      printf("����Ȧ����\r\n"); 
//    }
//    else
//    {
//      printf("����Ȧֵ��%d.%d.\r\n",reg[0],reg[1]);  
//    }
//   vTaskDelay(1);
   /*������0x0F д�����Ȧ*/
//   reg[0]=0x5555;
//   reg[1]=0xaaaa;
//   status= modbus_poll_write_multiple_coils(1,10,20,reg); 
//    
//   if(status!=MODBUS_POLL_STATUS_SUCCESS)
//   {
//     printf("д�����Ȧ����\r\n"); 
//   }
//     vTaskDelay(1);
//    status=modbus_poll_read_input_registers(1,20,10,reg);
//    
//    if(status!=MODBUS_POLL_STATUS_SUCCESS)
//    {
//      printf("������Ĵ�������\r\n"); 
//    }
//    else
//    {
//      for(i=0;i<10;i++)
//      {
//        printf("��ȡ������Ĵ���ֵ:%d.\r\n",reg[i]); 
//      }
//    }
//    vTaskDelay(1);
//    status= modbus_poll_write_single_coil(1,10,0);
//    if(status!=MODBUS_POLL_STATUS_SUCCESS)
//    {
//			printf("д������Ȧ����\r\n"); 
//    }
//    vTaskDelay(1);
   /*������0x10*/
//    status= modbus_poll_write_multiple_registers(1,0,10, reg);
//    if(status!=MODBUS_POLL_STATUS_SUCCESS)
//    {
//			printf("д����Ĵ�������\r\n"); 
//    }
     vTaskDelay(1);		
		LED1Toggle();
		SHT_GetValue();
		printf("\r\nTemp=%3.1lf  RH=%3.1lf \r\n",Tem_Value_1 ,RH_Value_1);
		vTaskDelay(500);			
  }
}   

//LED1������
void led1_task(void *pvParameters)
{
	  uint16_t reg[4];
  mb_poll_status_t status;
 // vTaskDelay(500);
	while(1)
	{
		LED2Toggle_GREEN();
					/*������0x03 �����ּĴ���*/
		status= modbus_poll_read_holding_registers(2,0,4,reg);
		if(status!=MODBUS_POLL_STATUS_SUCCESS)
		{
		 printf("�����ش���\r\n"); 
		}
		else
		{
	//		printf("������ֵ��1.%2x 2.%2x 3.%2x 4.%2x\r\n",reg[0],reg[1],reg[2],reg[3]);   
		}
		
	
		vTaskDelay(200);
		LED2Toggle_RED();
	}
}



void vTaskFunction( void *pvParameters )
{
		char *pcTaskName;
		volatile unsigned long ul;
		pcTaskName = ( char * ) pvParameters;
		taskENTER_CRITICAL(); 
/* As per most tasks, this task is implemented in an infinite loop. */
		for( ;; )
		{
/* Print out the name of this task. */
				printf( "%s \n",pcTaskName );
				vTaskDelay(1000);
/* Delay for a period. */
		for( ul = 0; ul < 500000; ul++ )
		{
/* This loop is just a very crude delay implementation. There is
nothing to do in here. Later exercises will replace this crude
loop with a proper delay/sleep function. */
		}
		taskEXIT_CRITICAL(); 
	}
		
}


