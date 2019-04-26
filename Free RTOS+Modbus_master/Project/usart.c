#include "usart.h"

/**-------------------------------------------------------
  * @������ __SZ_STM32_COMInit
  * @����   �����û���STM32��USART��ʼ������
  * @����1  COM1  ��ӦSTM32��USART1 ��Ӧ�������ϴ���1
  *         COM2  ��ӦSTM32��USART2 ��Ӧ�������ϴ���2
  * @����2  BaudRate ���ڵĲ����ʣ�����"115200"
  * @����ֵ ��
***------------------------------------------------------*/
void SZ_STM32_COMInit(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    /* ʹ��STM32��USART��ӦGPIO��Clockʱ�� */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO , ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 
    
	  GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);
    /* ��ʼ��STM32��USART��TX�ܽţ�����Ϊ���ù���������� */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB , &GPIO_InitStructure);

    /* ��ʼ��STM32��USART��RX�ܽţ�����Ϊ���ù������� */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOB , &GPIO_InitStructure);

	  USART_InitStructure.USART_BaudRate = 115200;              //���ڵĲ����ʣ�����115200 ��ߴ�4.5Mbits/s
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; //�����ֳ���(8λ��9λ)
    USART_InitStructure.USART_StopBits = USART_StopBits_1;      //�����õ�ֹͣλ-֧��1��2��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;         //����żУ��  
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ��������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //˫��ģʽ��ʹ�ܷ��ͺͽ���

    /* ���ݴ���Ĳ�����ʼ��STM32��USART���� */
    USART_Init(USART1, &USART_InitStructure);

    /* ʹ��STM32��USART����ģ�� */
    USART_Cmd(USART1, ENABLE);  
}


/* �ض���fputc���� ���ʹ��MicroLIBֻ��Ҫ�ض���fputc�������� */  
int fputc(int ch, FILE *f)
{
    /* e.g. write a character to the USART */
    USART_SendData(USART1, (uint8_t) ch);

    /* Loop until the end of transmission */
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);

    return ch;
}

int fgetc(FILE *fp)
{
	int ch = 0;
	
    while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
   
    ch = (int)USART1->DR & 0xFF;
	
    putchar(ch); //����
	
	return ch;
}
