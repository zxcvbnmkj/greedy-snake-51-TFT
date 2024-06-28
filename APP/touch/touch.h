#ifndef _touch_H
#define _touch_H


#include "public.h"


//---����ʹ�õ�IO��---//
sbit TOUCH_DOUT = P2^0;	  //���
sbit TOUCH_CLK  = P2^1;	  //ʱ��
sbit TOUCH_DIN  = P2^2;	  //����
sbit TOUCH_CS   = P2^3;	  //Ƭѡ
sbit TOUCH_PEN  = P2^4;	  //��ⴥ������Ӧ�ź�


//---����оƬ�����ֽ�---//
#define XPT_CMD_X	0xD0    //��ȡX�������
#define XPT_CMD_Y	0x90	//��ȡY�������

//---������������---//
typedef struct 
{
	u16 x;	 //��ӦXPT2046��ȡ��ADֵ
	u16 y;

	u32 lcdx;//��ӦTFTLCDʵ������ֵ
	u32 lcdy;

	u8 sta;//����״̬

} XPT_XY;

extern XPT_XY xpt_xy;

//---����ȫ�ֺ���---//
void TOUCH_SPI_Start(void);
void TOUCH_SPI_Write(u8 dat);
u16 TOUCH_SPI_Read(void);
u16 TOUCH_XPT_ReadData(u8 cmd);
u8 TOUCH_XPT_ReadXY(void);
u8 TOUCH_Scan(void);

#endif
