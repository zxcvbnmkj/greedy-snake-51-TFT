/*#include "touch.h"
#include "tftlcd.h"
#include "uart.h"

XPT_XY xpt_xy;	 //����һ��ȫ�ֱ�������X��Y��ֵ



void TOUCH_SPI_Start(void)
{
	TOUCH_CLK = 0;
	TOUCH_CS  = 1;
	TOUCH_DIN = 1;
	TOUCH_CLK = 1;
	TOUCH_CS  = 0;		
}

void TOUCH_SPI_Write(u8 dat)
{
	u8 i;
	TOUCH_CLK = 0;
	for(i=0; i<8; i++)
	{
		TOUCH_DIN = dat >>7;  	//�������λ
		dat <<= 1;
		TOUCH_CLK = 0;			//�����ط�������

		TOUCH_CLK = 1;

	}
}

u16 TOUCH_SPI_Read(void)
{
	u16 i, dat=0;
	TOUCH_CLK = 0;
	for(i=0; i<12; i++)		//����12λ����
	{
		dat <<= 1;

		TOUCH_CLK = 1;
		TOUCH_CLK = 0;

		dat |= TOUCH_DOUT;

	}
	return dat;	
}

#define XY_READ_TIMS  10		   //��ȡ�Ĵ���

u16 TOUCH_XPT_ReadData(u8 cmd)
{
	u8 i, j;
	u16 readValue[XY_READ_TIMS];
	long endValue;

	TOUCH_CLK = 0;    //������ʱ��
	TOUCH_CS  = 0;    //ѡ��оƬ
	
	for(i=0; i<XY_READ_TIMS; i++)	 	//��ȡXY_READ_TIMS�ν��
	{	
		TOUCH_SPI_Write(cmd);  //����ת������
		//delay_10us();
		for(j=6; j>0; j--); 	//��ʱ�ȴ�ת�����
		TOUCH_CLK = 1;	  //����һ��ʱ�����ڣ����BUSY
		_nop_();
		_nop_();
		TOUCH_CLK = 0;
		_nop_();
		_nop_();
	
		readValue[i] = TOUCH_SPI_Read();
	}
	TOUCH_CS = 1; //�ͷ�Ƭѡ

	//---����˲�---//
	//---�ȴ�С���򣬳�ȥ���ֵ����ȥ���ֵ������ƽ��ֵ---//
   	for(i=0; i<XY_READ_TIMS - 1; i++)	//�Ӵ�С����
	{
		for(j= i+1; j<XY_READ_TIMS; j++)
		{
			if(readValue[i] < readValue[j])
			{
				endValue = readValue[i];
				readValue[i] = readValue[j];
				readValue[j] = endValue;
			}  
		}
	}
//	if((readValue[2] - readValue[3]) > 5)
//	{
//		return 0;
//	}
	endValue = 0;
	for(i=2; i<XY_READ_TIMS-2; i++)		   
	{
		endValue += readValue[i]; 
	}
	endValue = endValue/ (XY_READ_TIMS - 4);//��ƽ��ֵ

	return endValue;
}



u8 TOUCH_XPT_ReadXY(void)
{	
	u16 x1, x2, x, y1, y2, y;

	TOUCH_SPI_Start();
	//---�ֱ������Xֵ��Yֵ, �����Ŷ��������һЩ��ȡ����---//
	x1 = TOUCH_XPT_ReadData(XPT_CMD_X);
	y1 = TOUCH_XPT_ReadData(XPT_CMD_Y);
	x2 = TOUCH_XPT_ReadData(XPT_CMD_X);
	y2 = TOUCH_XPT_ReadData(XPT_CMD_Y);	


	//---��ȡXֵ�Ĳ�ֵ---//
	if (x1 > x2)
	{
		x = x1 - x2;
	}
	else
	{
		x = x2 - x1;
	}

	//---��ȡYֵ�Ĳ�ֵ---//
	if (y1 > y2)
	{
		y = y1 - y2;
	}
	else
	{
		y = y2 - y1;
	}

	//---�жϲ�ֵ�Ƿ����50�����ھͷ���0����ʾ��ȡʧ��---//
	if((x > 50) || (y > 50))  
	{
		return 0;
	}

	//---��ȡ���ζ�ȡֵ��ƽ������Ϊ��ȡ����XYֵ---//
	xpt_xy.x = (x1 + x2) / 2;
	xpt_xy.y = (y1 + y2) / 2;

	xpt_xy.x &= 0xFFF0;	//ȥ������λ
	xpt_xy.y &= 0xFFF0;
	 
	//---ȷ��XYֵ�ķ�Χ�����ڴ���������TFTʱ---//
	if((xpt_xy.x < 100) || (xpt_xy.y > 3800))
	{
		return 0;
	}

	return 1;  // ����1����ʾ��ȡ�ɹ�
}

//����1����������
//0:�޴���
u8 TOUCH_Scan(void)
{
	u8 res=0;
	u32 temp;

	if(TOUCH_XPT_ReadXY())
	{
		//--�����������ʾ����ƫ�ƣ����Ը�����ʾADֵ--//
		//--�������湫ʽ�������ֵ--//
		if(tftlcd_data.dir==0)
		{
#ifdef TFT20_HX8309
			xpt_xy.lcdx = xpt_xy.x;
			xpt_xy.lcdx = (xpt_xy.lcdx - 250) * 180 / 3700; 
			xpt_xy.lcdy =	xpt_xy.y;
			xpt_xy.lcdy = (xpt_xy.lcdy - 350) * 250 / 3500;
#endif

#ifdef TFT22_ILI9225B
			xpt_xy.lcdx = xpt_xy.x;
			xpt_xy.lcdx = (xpt_xy.lcdx - 250) * 180 / 3700; 
			xpt_xy.lcdy =	xpt_xy.y;
			xpt_xy.lcdy = (xpt_xy.lcdy - 350) * 250 / 3500;
#endif

#ifdef TFT22_R61503B
			xpt_xy.lcdx = xpt_xy.x;
			xpt_xy.lcdx = (xpt_xy.lcdx - 250) * 180 / 3700; 
			xpt_xy.lcdy =	xpt_xy.y;
			xpt_xy.lcdy = (xpt_xy.lcdy - 350) * 250 / 3500;
#endif

#ifdef TFT24_ST7781R
			xpt_xy.lcdx=xpt_xy.x;
			xpt_xy.lcdx=((xpt_xy.lcdx - 336)*240)/3328;
			xpt_xy.lcdy=xpt_xy.y; 
			xpt_xy.lcdy =((xpt_xy.lcdy - 240) *320)/3412;
#endif	
	
#ifdef TFT26_R61509V
			xpt_xy.lcdx=xpt_xy.x;
			xpt_xy.lcdx=((xpt_xy.lcdx - 240)*260)/3850;
			xpt_xy.lcdy=xpt_xy.y; 
			xpt_xy.lcdy =((xpt_xy.lcdy - 200)*420)/3950;
#endif

#ifdef TFT26_ILI9325D
			xpt_xy.lcdx=xpt_xy.x;
			xpt_xy.lcdx=((xpt_xy.lcdx - 600)*297)/3550;
			xpt_xy.lcdy=xpt_xy.y; 
			xpt_xy.lcdy =((xpt_xy.lcdy - 250)*362)/3850;
#endif

		}
		else
		{
#ifdef TFT20_HX8309
		xpt_xy.lcdx = 4096-xpt_xy.y;
		xpt_xy.lcdx = (xpt_xy.lcdx - 350) * 250 / 3500;
		xpt_xy.lcdy = xpt_xy.x;
		xpt_xy.lcdy = (xpt_xy.lcdy - 250) * 180 / 3700;
#endif

#ifdef TFT22_ILI9225B
		xpt_xy.lcdx = 4096-xpt_xy.y;
		xpt_xy.lcdx = (xpt_xy.lcdx - 350) * 250 / 3500;
		xpt_xy.lcdy = xpt_xy.x;
		xpt_xy.lcdy = (xpt_xy.lcdy - 250) * 180 / 3700;
#endif

#ifdef TFT22_R61503B
		xpt_xy.lcdx = 4096-xpt_xy.y;
		xpt_xy.lcdx = (xpt_xy.lcdx - 350) * 250 / 3500;
		xpt_xy.lcdy = xpt_xy.x;
		xpt_xy.lcdy = (xpt_xy.lcdy - 250) * 180 / 3700;
#endif

#ifdef TFT24_ST7781R
			xpt_xy.lcdx=xpt_xy.y;
			xpt_xy.lcdx =((xpt_xy.lcdx - 240) *320)/3412;
			xpt_xy.lcdy=4096-xpt_xy.x; 
			xpt_xy.lcdy=((xpt_xy.lcdy - 336)*240)/3328;
#endif

#ifdef TFT26_R61509V
			xpt_xy.lcdx=xpt_xy.y;
			xpt_xy.lcdx=((xpt_xy.lcdx - 200)*420)/3950;
			xpt_xy.lcdy=4096 - xpt_xy.x; 
			xpt_xy.lcdy =((xpt_xy.lcdy - 240)*260)/3850;
#endif	

#ifdef TFT26_ILI9325D
			xpt_xy.lcdx=xpt_xy.y;
			xpt_xy.lcdx=((xpt_xy.lcdx - 250)*362)/3850;
			xpt_xy.lcdy=4096 - xpt_xy.x; 
			xpt_xy.lcdy =((xpt_xy.lcdy - 600)*297)/3550;
#endif	
					
		}
 

//		if(tftlcd_data.dir==0)
//		{
//			xpt_xy.lcdx=xpt_xy.lcdx;
//			xpt_xy.lcdy=xpt_xy.lcdy;	
//		}
//		else
//		{		
//			temp=xpt_xy.lcdx;
//			xpt_xy.lcdx=xpt_xy.lcdy;
//			xpt_xy.lcdy=tftlcd_data.height-temp;
//		}

//		printf("ad_x=%d   ad_y=%d\r\n",xpt_xy.x,xpt_xy.y);
//		printf("lcdx=%d   lcdy=%d\r\n",xpt_xy.lcdx,xpt_xy.lcdy);
	   res=1;
	   xpt_xy.sta=1;
	}
	else
		xpt_xy.sta=0;	
	return res;	
}
*/
