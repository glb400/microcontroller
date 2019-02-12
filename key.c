#include "key.h"

/**************************************************************************************
 * ��  �� : ����ɨ�躯��
 * ��  �� : ��
 * ����ֵ : �ĸ��������µĶ�Ӧֵ
 **************************************************************************************/
uint8 KeyScan(void)
{
	uint8 X_temp,Y_temp,temp;
	
	 X_temp=0xF0;        //��ֵ����ֵ
	 Y_temp=0x0F;        //��ֵ����ֵ

	 P2M1 &= 0x3F;	P2M0 |= 0xC0;	  //����P2.6~P2.7Ϊǿ�������
	 P4M1 &= 0x0F;	P4M0 |= 0xF0;	  //����P4.4~P4.7Ϊǿ�������
	 P5M1 &= 0xF3;	P5M0 |= 0x0C;	  //����P5.2~P5.3Ϊǿ�������

	
    ROW1=1;ROW2=1;ROW3=1;ROW4=1;  //���ø�
	  COL1=0;COL2=0;COL3=0;COL4=0;  //���õ�
	
		//���õ���IO������Ϊ���룬���м��
	  DelayMS(10);
	  P4M1 &= 0x0F;	P4M0 &= 0x0F;	  //����P4.4~P4.7Ϊ׼˫���
    DelayMS(10);
	
		if(ROW1 == 0)         //�����1��ƽ�Ƿ�Ϊ�͵�ƽ
		{
			DelayMS(10);
			if(ROW1 == 0)
			Y_temp &= 0x0E;	
		}
		if(ROW2 == 0)         //�����2��ƽ�Ƿ�Ϊ�͵�ƽ
		{
			DelayMS(10);
			if(ROW2 == 0)
			Y_temp &= 0x0D;	
		}
		if(ROW3 == 0)         //�����3��ƽ�Ƿ�Ϊ�͵�ƽ
		{
			DelayMS(10);
			if(ROW3 == 0)
			Y_temp &= 0x0B;	
		}
		if(ROW4 == 0)         //�����4��ƽ�Ƿ�Ϊ�͵�ƽ
		{
			DelayMS(10);
			if(ROW4 == 0)
			Y_temp &= 0x07;	
		}
	  
	  P2M1 &= 0x3F;	P2M0 |= 0xC0;	  //����P2.6~P2.7Ϊǿ�������
	  P4M1 &= 0x0F;	P4M0 |= 0xF0;	  //����P4.4~P4.7Ϊǿ�������
	  P5M1 &= 0xF3;	P5M0 |= 0x0C;	  //����P5.2~P5.3Ϊǿ�������
	
		ROW1=0;ROW2=0;ROW3=0;ROW4=0;  //���õ�
	  COL1=1;COL2=1;COL3=1;COL4=1;  //���ø�
	
		//���õ���IO������Ϊ���룬���м��
		DelayMS(10);
	  P2M1 &= 0x3F;	P2M0 &= 0x3F;	  //����P2.6~P2.7Ϊ׼˫���
	  P5M1 &= 0xF3;	P5M0 &= 0xF3;	  //����P5.2~P5.3Ϊ׼˫���
    DelayMS(10);
	
		if(COL1 == 0)         //�����1��ƽ�Ƿ�Ϊ�͵�ƽ
		{
			DelayMS(10);
			if(COL1 == 0)
			X_temp &= 0xE0;	
		}
		if(COL2 == 0)         //�����2��ƽ�Ƿ�Ϊ�͵�ƽ
		{
			DelayMS(10);
			if(COL2 == 0)
			X_temp &= 0xD0;	
		}
		if(COL3 == 0)         //�����3��ƽ�Ƿ�Ϊ�͵�ƽ
		{
			DelayMS(10);
			if(COL3 == 0)
			X_temp &= 0xB0;	
		}
		if(COL4 == 0)         //�����4��ƽ�Ƿ�Ϊ�͵�ƽ
		{
			DelayMS(10);
			if(COL4 == 0)
			X_temp &= 0x70;	
		}
		
		//����ֵ����ֵ�ϲ����õ�������Ӧ�ı���ֵ����ֵ��16������һһ��Ӧ
		temp = X_temp|Y_temp;
	  temp = ~temp;
			
		//����������ԭʼ����ֵ������Ӧ����ֵ��Ϣ
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