#include "rs485.h"
#include "delay.h"
#include "usart.h"
#include "mb_poll_port_serial.h"
#include "modbus_poll.h"

void UART5_IRQHandler(void)
{ uint16_t data;  
	if(USART_GetITStatus(UART5, USART_IT_RXNE) == SET)
	{	
//    data = USART_ReceiveData(UART5);	
//   // printf("0x%x",data);	
//    USART_SendData(USART1,data);		
   	mb_poll_byte_receive();	
		USART_ClearITPendingBit(UART5, USART_IT_RXNE);
	}

	
	if(USART_GetITStatus(UART5, USART_IT_TXE) == SET)
	{
		mb_poll_byte_send();
		USART_ClearFlag(UART5,USART_FLAG_TC);
	}
	
}

void RS485_init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure ;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure ;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC  ,ENABLE );
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD  ,ENABLE );
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5  ,ENABLE ); 
	
		//UART4_TX	  GPIOC.10��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	//UART4_RX	  GPIOC.11��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ�� 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 6 ;//��ռ���ȼ�8
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
	
	
	USART_InitStructure.USART_BaudRate = 9600;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//��У��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(UART5, &USART_InitStructure); //��ʼ������2
//  USART_ITConfig(UART5, USART_IT_RXNE | USART_IT_TXE, ENABLE);//�������ڽ����ж�
  USART_Cmd(UART5, ENABLE);   
}


void RS485_Send_Data(u8 *buf,u8 len)
{
	u8 t;
	RS485_TX_EN;			//����Ϊ����ģʽ
  	for(t=0;t<len;t++)		//ѭ����������
	{		   
		while(USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET);	  
		USART_SendData(UART5,buf[t]);
		//delay_ms();
	}	 
 
	while(USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET) ;		 
	RS485_RX_EN;				//����Ϊ����ģʽ	
}
