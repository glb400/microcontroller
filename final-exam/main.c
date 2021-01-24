#include "uart.h"		        	// ����ͨ�ź���ͷ�ļ�
#include "delay.h"	
#include "key.h"
/**********************
���ű�������
***********************/			
sbit RST = P2^0;     //��λ������IO��
sbit LED_B=P0^6;		 //��ɫLED��IO��P06,����ΪLED1	
sbit LED_R=P0^7;     //��ɫLED��IO��P07,����ΪLED2
 
/****************************************************************
ATָ�
*****************************************************************/	
char code str1[]="AT\r\n";                                    		    //  ����ָ�����"OK"
char code str2[]="AT+CWMODE=1\r\n";                         		      //  ����ESP8266�Ĺ���ģʽ������"OK"����"no change"
char code str3[]="AT+CWJAP=\"ChinaNet\",\"aaaaaaaa\"\r\n";     			      //  ���ӵ�WiFi�ȵ㣬ChinaNet aaaaaaaa   
char code str4[]="AT+CIFSR\r\n";                                   	  // 	����IP��ַ��ѯָ��
char code str5[]="AT+CIPSTART=\"TCP\",\"192.168.3.230\",5000\r\n";    //  ���ӵ�TCP�����������ء�Linked��
char code str6[]="AT+CIPSEND=6\r\n";   															  //  ��������ָ��
char code str7[]="hello!\r\n";  
char code str8[]="AT+CIPSERVER=1,5000\r\n";   												//  ����TCP�����������Ŷ˿�5000				
char code str9[]="AT+CIPMUX=1\r\n";   																//	�򿪶�����	
char code str10[]="AT+RST\r\n"; 																			//  �����λ
char code str11[]="AT+CIPSEND=0,15\r\n";   														//  ��������ָ��,���ڶ�·����ģʽ
char code str12[]="Command Executed!\r\n";   													//  ��������
char tmpl[]="x\r\n";
uint8 keymap[16] = {'1', '2', '3', 'A', '4', '5', '6', 'B', '7', '8', '9', 'C', '*', '0', '#', 'D'};
/***************************************************************************
 * ��  �� : ������
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************/
int main()		                                 
{	
	/////////////////////////////////////////////////
//ע��: STC15W4K32S4ϵ�е�оƬ,�ϵ��������PWM��ص�IO�ھ�Ϊ
//      ����̬,�轫��Щ������Ϊ׼˫��ڻ�ǿ����ģʽ��������ʹ��
//���IO: P0.6/P0.7/P1.6/P1.7/P2.1/P2.2
//        P2.3/P2.7/P3.7/P4.2/P4.4/P4.5
/////////////////////////////////////////////////
	uint8 key, last_key = 0, ch;
	P0M1 &= 0x3F;	P0M0 &= 0x3F;	            //����P0.6~P0.7Ϊ׼˫���
	P1M1 &= 0xFC;	P1M0 &= 0xFC;	            //����P1.0~P1.1Ϊ׼˫���(����2)
	P2M1 &= 0xFE;	P2M0 |= 0x01;	            //����P2.0Ϊ���������
	P3M1 &= 0xFC;	P3M0 &= 0xFC;	            //����P3.0~P3.1Ϊ׼˫���(����1)
      
	RST = 1;                                      //��λ�����ø�	
	UartInit();								                		// ��ʼ������1����2
	ES = 1;                                       // ����1�жϴ�
	IE2 |= 0x01;                                  // ����2�жϴ�
	EA = 1;                                       // ���жϴ�
	DelayMS(1000);							                  // ��ʱһ��ʱ�䣬��ESP8266����
	DelayUS(100);
	
	CLR_Buf();                                 		//�������2��������	

	while(!Hand("OK"))                            //�ж��Ƿ����ֳɹ�,������ɹ���ʱһ��,�ٷ���AT����ָ��
	{
		U2SendString(str1);                         //ͨ������2��������ָ�� 
		DelayMS(500);                               //��ʱ������
	}
	CLR_Buf();                                    //�������2��������	
	LED_R = 0;                                    //�����������ɫָʾ��

	while(!(Hand("OK")|Hand("no change")))        //�ж��Ƿ����óɹ����粻�ɹ�����ʱ���ٴη���
	{		
		U2SendString(str2);                         //��������ESP8266����ģʽָ��	
		DelayMS(500);		                            //��ʱ������
	}
	CLR_Buf();                                    //�������2��������	

	while(!Hand("OK"))    										 		//�ж��Ƿ�����WiFi·�������粻�ɹ�����ʱ���ٴη���
	{		
		U2SendString(str3); 	                      //���ӵ�WiFi�ȵ㣬lcesΪ�ȵ����ƣ�88518851Ϊ���룻���ӳɹ����ء�OK�� 
		DelayMS(2000);		                          //��ʱ������
	}
	LED_B = 0;                                    //������������ɫָʾ��
	CLR_Buf();                                    //�������2��������

	while(!Hand("OK"))                            //�ж��Ƿ�����TCP sever���粻�ɹ�����ʱ���ٴη���
	{
		U2SendString(str5);                         //���ӵ�TCP�����������ء�OK��
		DelayMS(3000);                              //��ʱ������
	}
	CLR_Buf();                                    //�������2��������			
	LED_R = 1;                                    //Ϩ�𿪷����ɫָʾ��
	LED_B = 1;                                    //Ϩ�𿪷�����ɫָʾ��
	
	while(!Hand("SEND OK"))     									//�ж��Ƿ������ݳɹ����粻�ɹ�����ʱ���ٴη���
	{

		U2SendString(str6);                         //���ݷ���ָ�� 
		DelayMS(100);		                            //��ʱ������
		U2SendString(str7);                         //��������	"hello!"
		DelayMS(500);                               //��ʱ������
	}
	CLR_Buf();                                    //�������2��������
	
	while (1) {
		key = KeyScan();
		if (UARTHand("success")) {
			while(!Hand("SEND OK"))     									//�ж��Ƿ������ݳɹ����粻�ɹ�����ʱ���ٴη���
			{
				U2SendString("AT+CIPSEND=7\r\n");                         //���ݷ���ָ�� 
				DelayMS(100);		                            //��ʱ������
				U2SendString("success\r\n");                
				DelayMS(500);                               //��ʱ������
			}
			CLR_Buf();     
		} else if (UARTHand("fail")) {
			while(!Hand("SEND OK"))     									//�ж��Ƿ������ݳɹ����粻�ɹ�����ʱ���ٴη���
			{

				U2SendString("AT+CIPSEND=4\r\n");                         //���ݷ���ָ�� 
				DelayMS(100);		                            //��ʱ������
				U2SendString("fail\r\n");                    
				DelayMS(500);                               //��ʱ������
			}
			CLR_Buf();                          
		} else if (UARTHand("close")) {
			while(!Hand("SEND OK"))     									//�ж��Ƿ������ݳɹ����粻�ɹ�����ʱ���ٴη���
			{

				U2SendString("AT+CIPSEND=5\r\n");                         //���ݷ���ָ�� 
				DelayMS(100);		                            //��ʱ������
				U2SendString("close\r\n");                    
				DelayMS(500);                               //��ʱ������
			}
			CLR_Buf();                          
		} else if (UARTHand("timeout")) {
			while(!Hand("SEND OK"))     									//�ж��Ƿ������ݳɹ����粻�ɹ�����ʱ���ٴη���
			{

				U2SendString("AT+CIPSEND=7\r\n");                         //���ݷ���ָ�� 
				DelayMS(100);		                            //��ʱ������
				U2SendString("timeout\r\n");                    
				DelayMS(500);                               //��ʱ������
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

