#include "uart.h"		        	// 串行通信函数头文件
#include "delay.h"	
#include "key.h"
/**********************
引脚别名定义
***********************/			
sbit RST = P2^0;     //复位引脚用IO口
sbit LED_B=P0^6;		 //蓝色LED用IO口P06,定义为LED1	
sbit LED_R=P0^7;     //红色LED用IO口P07,定义为LED2
 
/****************************************************************
AT指令集
*****************************************************************/	
char code str1[]="AT\r\n";                                    		    //  联机指令，返回"OK"
char code str2[]="AT+CWMODE=1\r\n";                         		      //  设置ESP8266的工作模式，返回"OK"或者"no change"
char code str3[]="AT+CWJAP=\"ChinaNet\",\"aaaaaaaa\"\r\n";     			      //  连接到WiFi热点，ChinaNet aaaaaaaa   
char code str4[]="AT+CIFSR\r\n";                                   	  // 	本机IP地址查询指令
char code str5[]="AT+CIPSTART=\"TCP\",\"192.168.3.230\",5000\r\n";    //  连接到TCP服务器，返回“Linked”
char code str6[]="AT+CIPSEND=6\r\n";   															  //  发送数据指令
char code str7[]="hello!\r\n";  
char code str8[]="AT+CIPSERVER=1,5000\r\n";   												//  建立TCP服务器，开放端口5000				
char code str9[]="AT+CIPMUX=1\r\n";   																//	打开多连接	
char code str10[]="AT+RST\r\n"; 																			//  软件复位
char code str11[]="AT+CIPSEND=0,15\r\n";   														//  发送数据指令,基于多路连接模式
char code str12[]="Command Executed!\r\n";   													//  数据内容
char tmpl[]="x\r\n";
uint8 keymap[16] = {'1', '2', '3', 'A', '4', '5', '6', 'B', '7', '8', '9', 'C', '*', '0', '#', 'D'};
/***************************************************************************
 * 描  述 : 主函数
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************/
int main()		                                 
{	
	/////////////////////////////////////////////////
//注意: STC15W4K32S4系列的芯片,上电后所有与PWM相关的IO口均为
//      高阻态,需将这些口设置为准双向口或强推挽模式方可正常使用
//相关IO: P0.6/P0.7/P1.6/P1.7/P2.1/P2.2
//        P2.3/P2.7/P3.7/P4.2/P4.4/P4.5
/////////////////////////////////////////////////
	uint8 key, last_key = 0, ch;
	P0M1 &= 0x3F;	P0M0 &= 0x3F;	            //设置P0.6~P0.7为准双向口
	P1M1 &= 0xFC;	P1M0 &= 0xFC;	            //设置P1.0~P1.1为准双向口(串口2)
	P2M1 &= 0xFE;	P2M0 |= 0x01;	            //设置P2.0为推挽输出口
	P3M1 &= 0xFC;	P3M0 &= 0xFC;	            //设置P3.0~P3.1为准双向口(串口1)
      
	RST = 1;                                      //复位引脚置高	
	UartInit();								                		// 初始化串口1串口2
	ES = 1;                                       // 串口1中断打开
	IE2 |= 0x01;                                  // 串口2中断打开
	EA = 1;                                       // 总中断打开
	DelayMS(1000);							                  // 延时一段时间，让ESP8266启动
	DelayUS(100);
	
	CLR_Buf();                                 		//清除串口2缓存内容	

	while(!Hand("OK"))                            //判断是否握手成功,如果不成功延时一会,再发送AT握手指令
	{
		U2SendString(str1);                         //通过串口2发送联机指令 
		DelayMS(500);                               //延时不可少
	}
	CLR_Buf();                                    //清除串口2缓存内容	
	LED_R = 0;                                    //点亮开发板红色指示灯

	while(!(Hand("OK")|Hand("no change")))        //判断是否设置成功，如不成功，延时后再次发送
	{		
		U2SendString(str2);                         //发送设置ESP8266工作模式指令	
		DelayMS(500);		                            //延时不可少
	}
	CLR_Buf();                                    //清除串口2缓存内容	

	while(!Hand("OK"))    										 		//判断是否连接WiFi路由器，如不成功，延时后再次发送
	{		
		U2SendString(str3); 	                      //连接到WiFi热点，lces为热点名称，88518851为密码；连接成功返回“OK” 
		DelayMS(2000);		                          //延时不可少
	}
	LED_B = 0;                                    //点亮开发板蓝色指示灯
	CLR_Buf();                                    //清除串口2缓存内容

	while(!Hand("OK"))                            //判断是否连接TCP sever，如不成功，延时后再次发送
	{
		U2SendString(str5);                         //连接到TCP服务器，返回“OK”
		DelayMS(3000);                              //延时不可少
	}
	CLR_Buf();                                    //清除串口2缓存内容			
	LED_R = 1;                                    //熄灭开发板红色指示灯
	LED_B = 1;                                    //熄灭开发板蓝色指示灯
	
	while(!Hand("SEND OK"))     									//判断是否发送数据成功，如不成功，延时后再次发送
	{

		U2SendString(str6);                         //数据发送指令 
		DelayMS(100);		                            //延时不可少
		U2SendString(str7);                         //数据内容	"hello!"
		DelayMS(500);                               //延时不可少
	}
	CLR_Buf();                                    //清除串口2缓存内容
	
	while (1) {
		key = KeyScan();
		if (UARTHand("success")) {
			while(!Hand("SEND OK"))     									//判断是否发送数据成功，如不成功，延时后再次发送
			{
				U2SendString("AT+CIPSEND=7\r\n");                         //数据发送指令 
				DelayMS(100);		                            //延时不可少
				U2SendString("success\r\n");                
				DelayMS(500);                               //延时不可少
			}
			CLR_Buf();     
		} else if (UARTHand("fail")) {
			while(!Hand("SEND OK"))     									//判断是否发送数据成功，如不成功，延时后再次发送
			{

				U2SendString("AT+CIPSEND=4\r\n");                         //数据发送指令 
				DelayMS(100);		                            //延时不可少
				U2SendString("fail\r\n");                    
				DelayMS(500);                               //延时不可少
			}
			CLR_Buf();                          
		} else if (UARTHand("close")) {
			while(!Hand("SEND OK"))     									//判断是否发送数据成功，如不成功，延时后再次发送
			{

				U2SendString("AT+CIPSEND=5\r\n");                         //数据发送指令 
				DelayMS(100);		                            //延时不可少
				U2SendString("close\r\n");                    
				DelayMS(500);                               //延时不可少
			}
			CLR_Buf();                          
		} else if (UARTHand("timeout")) {
			while(!Hand("SEND OK"))     									//判断是否发送数据成功，如不成功，延时后再次发送
			{

				U2SendString("AT+CIPSEND=7\r\n");                         //数据发送指令 
				DelayMS(100);		                            //延时不可少
				U2SendString("timeout\r\n");                    
				DelayMS(500);                               //延时不可少
			}
			CLR_Buf();                          
		}
		
		if (last_key != 0 && last_key == key) continue;
		last_key = key;
		ch = keymap[key - 1];
		tmpl[0] = ch;
		if (key)
			U1SendData(ch);
	}
}

