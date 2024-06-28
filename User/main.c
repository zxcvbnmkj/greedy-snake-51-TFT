#include "public.h"  //����ÿһ�ڵ�����λ�úʹ�ӡλ�ã�����=ǰ��-3���������궼��
#include "tftlcd.h"
#include "gui.h"
#include "stdlib.h"
#define SNAKE_Max_Long 100
#define SNAKE_head_x 88//��ӡ84+1,/7=12
#define SNAKE_head_y 176//��ӡ154+19,/7=22
#define SNAKE_firstBody_x 88//��ӡ84+1
#define SNAKE_firstBody_y 183//��ӡ161+19,/7=23
#define rect_start_x 0  // w(1,175),h(19,200)
#define rect_start_y 18
#define rect_width 176  //174,25��.����*7-1=���س���
#define rect_height 183//181,26��
#define color RED
#define SPEED 5000
//#define headcolor GREEN
//sbit key1=P0^0;//��
//sbit key2=P0^1;//��
//sbit key3=P0^2;//��
//sbit key4=P0^3;//��
//sbit key5=P0^4;//���¿�ʼ
unsigned char direction;//���򣬳�ʼΪ1��Ϊ1�������ϣ�2�£�3 ��4��
u8 directionBackUP;//����ı��ݣ��洢��һ�εķ���
struct 
{
	u8 X[SNAKE_Max_Long];//���ڵĺ�����
	u8 Y[SNAKE_Max_Long];//���ڵ�������
	u8 Long;  
	u8 Self;//�Ƿ�ײ���Լ���0Ϊû�У�1�� 
	u8 Score;
  u8 headX;//ͷ�ĺ�����
  u8 headY;	//ͷ��������
}Snake;//�߽ṹ��

struct 
{
	u8 printX;
	u8 printY;
  u8	numx;
	u8 numy;
}Food;	//ʳ��ṹ��
	
void showInit()
{
	Snake.headX=SNAKE_head_x;//���¶Խṹ�帳ֵ
	Snake.headY=SNAKE_head_y;
	Snake.Score=0;
	Snake.Self=0;
	Snake.Long=2;
	Snake.X[0]=SNAKE_head_x;
	Snake.Y[0]=SNAKE_head_y;
	Snake.X[1]=SNAKE_firstBody_x;
	Snake.Y[1]=SNAKE_firstBody_y;
	
	TFTLCD_Init();
  FRONT_COLOR=RED;//��174����25������,���0~24����181��26�񣬱��0~25
	LCD_ShowString(4,0,tftlcd_data.width,tftlcd_data.height,16,"Greedy Snake Game");//y:0~15
	gui_draw_rectangle(rect_start_x,rect_start_y,rect_width,rect_height,BLUE);//����һ�����ο�(1,19)
	LCD_ShowString(4,203,tftlcd_data.width,tftlcd_data.height,16,"Score:");//��Ļy������0~219
	LCD_ShowNum(50,203,Snake.Score,2,16);
	gui_fill_rectangle(SNAKE_head_x-3,SNAKE_head_y-3,6,6,color);//��ӡͷ
	gui_fill_rectangle(SNAKE_firstBody_x-3,SNAKE_firstBody_y-3,6,6,color);//��ӡһ������
}
void Time0_Init()          //��ʱ����ʼ��
{
	TMOD=0X21;//��ʱ��0Ϊ0001����ʱ��1Ϊ0010��TMOD����λѰַ������������һ���Ը�ֵ��   
	EA=1;//�����ж�
	ET0=1;//�򿪶�ʱ��0���ж�
	TH0  = 0xfe;
	TL0  = 0x33; //11.0592MZ����0.5ms
	TR0=1; //��ʱ����ʼ
}
void foodProduce()
{
	int f,u;
	do
	{
	f=0;
	Food.numx=TL1%25;//��n������0��n-1����,0~24
	Food.numy=TL1%26;//0~25
	Food.printX=Food.numx*7+1;
	Food.printY=Food.numy*7+19;
	for(u=0;u<Snake.Long-1;u++)
		if(Food.printX==Snake.X[u]&&Food.printY==Snake.Y[u])f=1;
	}while(f);//���Ƿ�ֹ�ո�ʳ������غ�
	gui_fill_rectangle(Food.printX,Food.printY,6,6,GRED);
}
//void keyout()
//{
//	unsigned int a;
//	key1=1;
//	key2=1;
//	key3=1;
//	key4=1;
//	//while(key1==1&&key2==1&&key3==1&&key4==1);//δ������ͣ����
//	if(key1==0||key2==0||key3==0||key4==0)//key=0˵��������������
//	{
//		for(a=0;a<1000;a++);//��ʱԼ10ms
//		if(key1==0||key2==0||key3==0||key4==0)//�ٴμ���Ƿ�����а�������
//		{
//			if(key1==0){direction=1;}
//			if(key2==0){direction=2;}
//			if(key3==0){direction=3;}
//			if(key4==0){direction=4;}
//		}
//		while(!(key1==1&&key2==1&&key3==1&&key4==1));//ֻҪȫ��������ƽ��Ϊ1�ſ�ͨ����ѭ��
//		for(a=0;a<1000;a++);//��ʱԼ10ms
//		while(!(key1==1&&key2==1&&key3==1&&key4==1));//�ٴμ��
//	}
//}
void Display_dead()//��ʾ��������
{
	LCD_Clear(BLACK);
	LCD_ShowString(30,70,tftlcd_data.width,tftlcd_data.height,24,"GAME OVER!");
	LCD_ShowString(20,110,tftlcd_data.width,tftlcd_data.height,16,"YOUR SORCE IS");
	LCD_ShowNum(130,105,Snake.Score,2,24);
	LCD_ShowString(0,135,tftlcd_data.width,tftlcd_data.height,12,"Press K1 to restart the game!");
}
void snakeMove()
{
	u8 last_x,last_y,count,e;
	direction=1;
	while(Snake.headX-3>rect_start_x&&Snake.headX+3<rect_start_x+rect_width&&Snake.headY-3>rect_start_y&&Snake.headY+3<rect_start_y+rect_height&&Snake.Self==0)
	{//����δԽ��
		directionBackUP=direction;
		//keyout();
		
		//�ѷ���ָ���ԭ���ķ����ˣ������ڣ��˴ΰ��µİ�����Ч
		if(direction==1&&Snake.Y[0]-Snake.Y[1]==7)direction=directionBackUP;
		if(direction==2&&Snake.Y[0]-Snake.Y[1]==-7)direction=directionBackUP;
		if(direction==3&&Snake.X[0]-Snake.X[1]==7)direction=directionBackUP;
		if(direction==4&&Snake.X[0]-Snake.X[1]==-7)direction=directionBackUP;
		
		switch(direction)
		{
			case 1:{Snake.headY=Snake.headY-7;break;}
			case 2:{Snake.headY=Snake.headY+7;break;}
			case 3:{Snake.headX=Snake.headX-7;break;}
			case 4:{Snake.headX=Snake.headX+7;break;}
		}
		last_x=Snake.X[Snake.Long-1];last_y=Snake.Y[Snake.Long-1];//���������һ�ڵ�����
		for(count=1;count<Snake.Long;count++)//count=n-1ʱѭ����������Ϊ��ʱa[n-count-1]Ϊa[0]
		{
			Snake.X[Snake.Long-count]=Snake.X[Snake.Long-count-1]; //a[n-1]�����һ�ڣ��������ǵ����ڼ���
			Snake.Y[Snake.Long-count]=Snake.Y[Snake.Long-count-1];
		}
		Snake.X[0]=Snake.headX;Snake.Y[0]=Snake.headY;
		gui_fill_rectangle(Snake.headX-3,Snake.headY-3,6,6,color);//��ӡͷ
		//gui_fill_rectangle(Snake.X[1]-3,Snake.Y[1]-3,6,6,color);//��ӡ�����ͷ�ǵ�
		
		if(!(Snake.headX==Food.printX+3&&Snake.headY==Food.printY+3))//����ͷ���겻����ʳ�����꣬�Ͱ����һ�����ڵ�
		{
			 gui_fill_rectangle(last_x-3,last_y-3,6,6,BLACK);
		}
		else
		{
			Snake.Long++;
			Snake.Score++;
			foodProduce();
			LCD_ShowNum(50,203,Snake.Score,2,16);
			Snake.X[Snake.Long-1]=last_x;
			Snake.Y[Snake.Long-1]=last_y;
			
		}
		for(e=4;e<Snake.Long;e++)//�����ͷ�����Ƿ������������غϣ������self��1
			if(Snake.headX==Snake.X[e]&&Snake.headY==Snake.Y[e])//e=3,�ӵ���ڿ�ʼ��飬��Ϊ��ֻ��4��������ô��������ײ���Լ�
			{Snake.Self=1;break;}
		delay_ms(SPEED);
	}
	Display_dead();
}
void initUART()//������9600
{
	SCON=0x50;
	TH1=0XFA;
	TL1=TH1;
	PCON=0X80;//ͨ���������SMODλΪ1
	ES=1;
//	EA=1;���ж���֮ǰ�ĺ���Time0_Init()���Ѵ򿪣���������ע�͵�
	ET1=0;
	TR1=1;
}
void main()
{
  Time0_Init();
	initUART();
	showInit();
	foodProduce();
	snakeMove();
//	key5=1;
	while(1);
}
void Time0_Int() interrupt 1     //��ʱ��0�ж�,TF0=1ʱ���������ȼ��ȴ����жϸ�
{
	TH0  = 0xfe;                 //���¸�ֵ
	TL0  = 0x33;
}
void uart() interrupt 4//RI=1ʱ����
{
	unsigned char i;
	if(RI==1)
	{
		RI=0;
		i=SBUF;
	}
	switch(i)
	{
		case 0x01:{direction=1;break;}
		case 0x02:{direction=2;break;}
		case 0x03:{direction=3;break;}
		case 0x04:{direction=4;break;}
	}
	SBUF=direction;
	while(!TI);
	TI=0;
}