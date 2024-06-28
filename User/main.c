#include "public.h"  //区分每一节的坐标位置和打印位置，后者=前者-3，横纵坐标都是
#include "tftlcd.h"
#include "gui.h"
#include "stdlib.h"
#define SNAKE_Max_Long 100
#define SNAKE_head_x 88//打印84+1,/7=12
#define SNAKE_head_y 176//打印154+19,/7=22
#define SNAKE_firstBody_x 88//打印84+1
#define SNAKE_firstBody_y 183//打印161+19,/7=23
#define rect_start_x 0  // w(1,175),h(19,200)
#define rect_start_y 18
#define rect_width 176  //174,25格.格数*7-1=像素长度
#define rect_height 183//181,26格
#define color RED
#define SPEED 5000
//#define headcolor GREEN
//sbit key1=P0^0;//上
//sbit key2=P0^1;//下
//sbit key3=P0^2;//左
//sbit key4=P0^3;//右
//sbit key5=P0^4;//重新开始
unsigned char direction;//方向，初始为1。为1代表向上，2下，3 左，4右
u8 directionBackUP;//方向的备份，存储上一次的方向
struct 
{
	u8 X[SNAKE_Max_Long];//各节的横坐标
	u8 Y[SNAKE_Max_Long];//各节的纵坐标
	u8 Long;  
	u8 Self;//是否撞到自己，0为没有，1有 
	u8 Score;
  u8 headX;//头的横坐标
  u8 headY;	//头的纵坐标
}Snake;//蛇结构体

struct 
{
	u8 printX;
	u8 printY;
  u8	numx;
	u8 numy;
}Food;	//食物结构体
	
void showInit()
{
	Snake.headX=SNAKE_head_x;//以下对结构体赋值
	Snake.headY=SNAKE_head_y;
	Snake.Score=0;
	Snake.Self=0;
	Snake.Long=2;
	Snake.X[0]=SNAKE_head_x;
	Snake.Y[0]=SNAKE_head_y;
	Snake.X[1]=SNAKE_firstBody_x;
	Snake.Y[1]=SNAKE_firstBody_y;
	
	TFTLCD_Init();
  FRONT_COLOR=RED;//宽174，有25个格子,编号0~24。高181，26格，编号0~25
	LCD_ShowString(4,0,tftlcd_data.width,tftlcd_data.height,16,"Greedy Snake Game");//y:0~15
	gui_draw_rectangle(rect_start_x,rect_start_y,rect_width,rect_height,BLUE);//绘制一个矩形框(1,19)
	LCD_ShowString(4,203,tftlcd_data.width,tftlcd_data.height,16,"Score:");//屏幕y坐标是0~219
	LCD_ShowNum(50,203,Snake.Score,2,16);
	gui_fill_rectangle(SNAKE_head_x-3,SNAKE_head_y-3,6,6,color);//打印头
	gui_fill_rectangle(SNAKE_firstBody_x-3,SNAKE_firstBody_y-3,6,6,color);//打印一节身体
}
void Time0_Init()          //定时器初始化
{
	TMOD=0X21;//定时器0为0001，定时器1为0010。TMOD不能位寻址，所以在这里一次性赋值。   
	EA=1;//打开总中断
	ET0=1;//打开定时器0的中断
	TH0  = 0xfe;
	TL0  = 0x33; //11.0592MZ晶振，0.5ms
	TR0=1; //定时器开始
}
void foodProduce()
{
	int f,u;
	do
	{
	f=0;
	Food.numx=TL1%25;//余n即产生0到n-1的数,0~24
	Food.numy=TL1%26;//0~25
	Food.printX=Food.numx*7+1;
	Food.printY=Food.numy*7+19;
	for(u=0;u<Snake.Long-1;u++)
		if(Food.printX==Snake.X[u]&&Food.printY==Snake.Y[u])f=1;
	}while(f);//这是防止刚刚食物和蛇重合
	gui_fill_rectangle(Food.printX,Food.printY,6,6,GRED);
}
//void keyout()
//{
//	unsigned int a;
//	key1=1;
//	key2=1;
//	key3=1;
//	key4=1;
//	//while(key1==1&&key2==1&&key3==1&&key4==1);//未按下则停在这
//	if(key1==0||key2==0||key3==0||key4==0)//key=0说明按键被按下了
//	{
//		for(a=0;a<1000;a++);//延时约10ms
//		if(key1==0||key2==0||key3==0||key4==0)//再次检测是否真的有按键按下
//		{
//			if(key1==0){direction=1;}
//			if(key2==0){direction=2;}
//			if(key3==0){direction=3;}
//			if(key4==0){direction=4;}
//		}
//		while(!(key1==1&&key2==1&&key3==1&&key4==1));//只要全部按键电平都为1才可通过该循环
//		for(a=0;a<1000;a++);//延时约10ms
//		while(!(key1==1&&key2==1&&key3==1&&key4==1));//再次检测
//	}
//}
void Display_dead()//显示死亡界面
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
	{//若蛇未越界
		directionBackUP=direction;
		//keyout();
		
		//把方向恢复成原来的方向了，即等于，此次按下的按键无效
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
		last_x=Snake.X[Snake.Long-1];last_y=Snake.Y[Snake.Long-1];//保存起最后一节的坐标
		for(count=1;count<Snake.Long;count++)//count=n-1时循环结束，因为此时a[n-count-1]为a[0]
		{
			Snake.X[Snake.Long-count]=Snake.X[Snake.Long-count-1]; //a[n-1]是最后一节，减几就是倒数第几节
			Snake.Y[Snake.Long-count]=Snake.Y[Snake.Long-count-1];
		}
		Snake.X[0]=Snake.headX;Snake.Y[0]=Snake.headY;
		gui_fill_rectangle(Snake.headX-3,Snake.headY-3,6,6,color);//打印头
		//gui_fill_rectangle(Snake.X[1]-3,Snake.Y[1]-3,6,6,color);//打印身体把头盖掉
		
		if(!(Snake.headX==Food.printX+3&&Snake.headY==Food.printY+3))//若蛇头坐标不等于食物坐标，就把最后一节蛇遮掉
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
		for(e=4;e<Snake.Long;e++)//检查蛇头坐标是否与蛇身坐标重合，有则把self置1
			if(Snake.headX==Snake.X[e]&&Snake.headY==Snake.Y[e])//e=3,从第五节开始检查，因为若只有4节无论怎么都不可能撞到自己
			{Snake.Self=1;break;}
		delay_ms(SPEED);
	}
	Display_dead();
}
void initUART()//波特率9600
{
	SCON=0x50;
	TH1=0XFA;
	TL1=TH1;
	PCON=0X80;//通过这个控制SMOD位为1
	ES=1;
//	EA=1;总中断在之前的函数Time0_Init()中已打开，所以这行注释掉
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
void Time0_Int() interrupt 1     //定时器0中断,TF0=1时触发，优先级比串口中断高
{
	TH0  = 0xfe;                 //重新赋值
	TL0  = 0x33;
}
void uart() interrupt 4//RI=1时触发
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