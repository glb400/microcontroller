#include "key.h"

/**************************************************************************************
 * 描  述 : 按键扫描函数
 * 入  参 : 无
 * 返回值 : 哪个按键按下的对应值
 **************************************************************************************/
uint8 KeyScan(void)
{
	uint8 X_temp,Y_temp,temp;
	
	 X_temp=0xF0;        //列值赋初值
	 Y_temp=0x0F;        //行值赋初值

	 P2M1 &= 0x3F;	P2M0 |= 0xC0;	  //设置P2.6~P2.7为强推挽输出
	 P4M1 &= 0x0F;	P4M0 |= 0xF0;	  //设置P4.4~P4.7为强推挽输出
	 P5M1 &= 0xF3;	P5M0 |= 0x0C;	  //设置P5.2~P5.3为强推挽输出

	
    ROW1=1;ROW2=1;ROW3=1;ROW4=1;  //行置高
	  COL1=0;COL2=0;COL3=0;COL4=0;  //列置低
	
		//所用到行IO口配置为输入，进行检测
	  DelayMS(10);
	  P4M1 &= 0x0F;	P4M0 &= 0x0F;	  //设置P4.4~P4.7为准双向口
    DelayMS(10);
	
		if(ROW1 == 0)         //检测行1电平是否为低电平
		{
			DelayMS(10);
			if(ROW1 == 0)
			Y_temp &= 0x0E;	
		}
		if(ROW2 == 0)         //检测行2电平是否为低电平
		{
			DelayMS(10);
			if(ROW2 == 0)
			Y_temp &= 0x0D;	
		}
		if(ROW3 == 0)         //检测行3电平是否为低电平
		{
			DelayMS(10);
			if(ROW3 == 0)
			Y_temp &= 0x0B;	
		}
		if(ROW4 == 0)         //检测行4电平是否为低电平
		{
			DelayMS(10);
			if(ROW4 == 0)
			Y_temp &= 0x07;	
		}
	  
	  P2M1 &= 0x3F;	P2M0 |= 0xC0;	  //设置P2.6~P2.7为强推挽输出
	  P4M1 &= 0x0F;	P4M0 |= 0xF0;	  //设置P4.4~P4.7为强推挽输出
	  P5M1 &= 0xF3;	P5M0 |= 0x0C;	  //设置P5.2~P5.3为强推挽输出
	
		ROW1=0;ROW2=0;ROW3=0;ROW4=0;  //行置低
	  COL1=1;COL2=1;COL3=1;COL4=1;  //列置高
	
		//所用到列IO口配置为输入，进行检测
		DelayMS(10);
	  P2M1 &= 0x3F;	P2M0 &= 0x3F;	  //设置P2.6~P2.7为准双向口
	  P5M1 &= 0xF3;	P5M0 &= 0xF3;	  //设置P5.2~P5.3为准双向口
    DelayMS(10);
	
		if(COL1 == 0)         //检测列1电平是否为低电平
		{
			DelayMS(10);
			if(COL1 == 0)
			X_temp &= 0xE0;	
		}
		if(COL2 == 0)         //检测列2电平是否为低电平
		{
			DelayMS(10);
			if(COL2 == 0)
			X_temp &= 0xD0;	
		}
		if(COL3 == 0)         //检测列3电平是否为低电平
		{
			DelayMS(10);
			if(COL3 == 0)
			X_temp &= 0xB0;	
		}
		if(COL4 == 0)         //检测列4电平是否为低电平
		{
			DelayMS(10);
			if(COL4 == 0)
			X_temp &= 0x70;	
		}
		
		//将行值和列值合并，得到按键对应的编码值，该值与16个按键一一对应
		temp = X_temp|Y_temp;
	  temp = ~temp;
			
		//将按键检测的原始编码值解析对应按键值信息
    switch (temp)
    {   
		      case 0x11:return 1;	//1
		      case 0x21:return 2;	//2	
		      case 0x41:return 3;	//3
		      case 0x81:return 4;	//4
		      case 0x12:return 5;	//5
		      case 0x22:return 6;	//6
		      case 0x42:return 7;	//7
		      case 0x82:return 8;	//8
		      case 0x14:return 9;	//9
		      case 0x24:return 10;	//0
		      case 0x44:return 11;	//a
		      case 0x84:return 12;	//b
		      case 0x18:return 13;	//c
		      case 0x28:return 14;	//d
		      case 0x48:return 15;  //e
		      case 0x88:return 16;	//f	
		      default:  return 0; 
    }                 
} 