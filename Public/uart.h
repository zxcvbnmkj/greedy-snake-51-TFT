#ifndef _uart_H
#define _uart_H

#include "public.h"
#include "stdio.h"


#define UART_REC_LEN  			50  	//�����������ֽ��� 50

extern u8  UART_RX_BUF[UART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 UART_RX_STA;         		//����״̬���	


void UART_Init(void);
void UART_Cmd(FunctionalState ensta);
void UART_SendData(u8 dat);
void UART_SendString(u8 *pbuf);


#endif
