#ifndef _tftlcd_H
#define _tftlcd_H

#include "public.h"

//����LCD��������������  �ɸ����Լ����ϵĲ��������ͺ���ѡ�����������
//#define TFT22_ILI9225B

//#define TFT22_ILI9340

//#define TFT24_ST7781R

//#define TFT26_R61509V

//#define TFT26_ILI9325D

#define TFT22_R61503B

//#define TFT20_HX8309


//TFTLCD��ʾ�������
#define TFTLCD_DIR	0	//0������  1������  Ĭ������


//�ֱ���
#ifdef TFT20_HX8309
	#define	WIDTH	176
	#define	HEIGHT	220
#endif

#ifdef TFT22_R61503B
	#define	WIDTH	176
	#define	HEIGHT	220
#endif

#ifdef TFT22_ILI9225B
	#define	WIDTH	176
	#define	HEIGHT	220
#endif

#ifdef TFT24_ST7781R
	#define	WIDTH	240
	#define	HEIGHT	320
#endif

#ifdef TFT26_R61509V
	#define	WIDTH	240
	#define	HEIGHT	400
#endif

#ifdef TFT26_ILI9325D
	#define	WIDTH	240
	#define	HEIGHT	320
#endif

//TFTLCD�������ݿ��ƶ˿ڶ���
#define TFT_DATAPORTH P1
#define TFT_DATAPORTL P0

sbit	TFT_CS  = P2^7;
sbit	TFT_RST = P3^3;
sbit	TFT_RS  = P2^6;
sbit	TFT_WR  = P2^5;
sbit	TFT_RD  = P3^2;

//TFTLCD��Ҫ������
typedef struct  
{										    
	u16 width;			//LCD ���
	u16 height;			//LCD �߶�
	u16 id;				//LCD ID
	u8  dir;            //LCD ����
}_tftlcd_data;


//LCD����
extern _tftlcd_data tftlcd_data;	//����LCD��Ҫ����
//LCD��ǰ����ɫ�ͱ���ɫ	   
extern u16  FRONT_COLOR;//ǰ����ɫ Ĭ�Ϻ�ɫ    
extern u16  BACK_COLOR; //������ɫ.Ĭ��Ϊ��ɫ


//������ɫ
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ



void LCD_WriteCmd(u16 cmd);
void LCD_WriteData(u16 dat);
void LCD_WriteCmdData(u16 cmd,u16 dat);
void LCD_WriteData_Color(u16 color);

void TFTLCD_Init(void);           //��ʼ��
void LCD_Set_Window(u16 sx,u16 sy,u16 width,u16 height);
void LCD_Clear(u16 Color);	 												//����
void LCD_Fill(u16 xState,u16 yState,u16 xEnd,u16 yEnd,u16 color);
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color);
void LCD_DrawPoint(u16 x,u16 y);//����
void LCD_DrawFRONT_COLOR(u16 x,u16 y,u16 color);
void LCD_DrawBigPoint(u16 x,u16 y,u16 color);
u16 LCD_ReadPoint(u16 x,u16 y);
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_DrawLine_Color(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);
void LCD_DrowSign(u16 x, u16 y, u16 color);
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r);
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p);
void LCD_ShowFontHZ(u16 x, u16 y, u8 *cn);					   						   																			
void LCD_ShowPicture(u16 x, u16 y, u16 wide, u16 high,u8 *pic);


#endif
