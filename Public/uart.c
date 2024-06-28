/*#include "uart.h"

#define RELOAD_COUNT 0xFA //�궨�岨���ʷ�����������ֵ 9600

void UART_Init(void)
{
	SCON=0X50;			//����Ϊ������ʽ1
	TMOD=0X20;			//���ü�����������ʽ2
	PCON=0X80;			//�����ʼӱ�
	TH1=RELOAD_COUNT;	//��������ʼֵ����
	TL1=TH1;
	//ES=1;				//�رս����ж�
	EA=1;				//�����ж�
	TR1=1;				//�򿪼�����
	TI=1;          //�����жϱ��λ�����ʹ��printf�����ı�������	
}

void UART_Cmd(FunctionalState ensta)
{
	ES=ensta;	
}

void UART_SendData(u8 dat)
{
	ES=0; //�رմ����ж�
	TI=0; //�巢������ж������־λ
	SBUF=dat; //����
	while(TI==0); //�ȴ��������
	TI=0; //�巢������ж������־λ
	ES=1; //�������ж�
}

void UART_SendString(u8 *pbuf)
{
	while(*pbuf!='\0') //�����ո�����ѭ��	
	{
		UART_SendData(*pbuf);
		delay_10us(5);
		pbuf++;	
	}
}

u8 UART_RX_BUF[UART_REC_LEN];     //���ջ���,���UART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 UART_RX_STA=0;       //����״̬���	


void UART_IRQn() interrupt 4
{
	u8 r;
	
	if(RI)
	{
		RI=0;
		r=SBUF;//��ȡ���յ�������
		if((UART_RX_STA&0x8000)==0)//����δ���
		{
			if(UART_RX_STA&0x4000)//���յ���0x0d
			{
				if(r!=0x0a)UART_RX_STA=0;//���մ���,���¿�ʼ
				else UART_RX_STA|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(r==0x0d)UART_RX_STA|=0x4000;
				else
				{
					UART_RX_BUF[UART_RX_STA&0X3FFF]=r ;
					UART_RX_STA++;
					if(UART_RX_STA>(UART_REC_LEN-1))UART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}   		
	}	
}
*/
