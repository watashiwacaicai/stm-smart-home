#include "stm32f10x.h"                  // Device header
#include "delay.h"

unsigned char tempL;     //��ȫ�ֱ���
unsigned char tempH; 
unsigned int sdata;    //�¶ȵĲ���
unsigned char fg;

void DS_DIR_IN(void)  //��PB7Ϊ��������ģʽ
{
	
	GPIO_InitTypeDef kong;	
	kong.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	kong.GPIO_Pin = GPIO_Pin_7;
	kong.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&kong);
}
 

void DS_DIR_OUT(void) //��PB7Ϊ�������ģʽ
{

	GPIO_InitTypeDef GPIO_outt;
	GPIO_outt.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_outt.GPIO_Pin = GPIO_Pin_7;
	GPIO_outt.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOB,&GPIO_outt);
}

void Init_DS18B20(void) 
{
	unsigned char t = 0;
	DS_DIR_OUT();  //��GPIO��Ϊ�������ģʽ
	GPIO_ResetBits(GPIOB,GPIO_Pin_7);  //���͸�λ���� ds18b20 DQ�ܽŽӵ���Ƭ����PB7
	Delay_us(600); 		//��ʱ��>480us) 
	GPIO_SetBits(GPIOB,GPIO_Pin_7);		//���������� 
	Delay_us(55); 				//�ȴ���15~60us)	
	DS_DIR_IN(); //����GPIO��Ϊ��������ģʽ
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7) == SET) //�ȴ�����
	{
		Delay_us(1);
		t++;
		if(t >= 240)//�������240us���Ǹߵ�ƽ ����ds18b20û�����ݻ��� ��ʧ������
			return;   
	}
	t = 0;
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7) == RESET) //�ȴ�����
	{
		Delay_us(1);
		t++;
		if(t >= 240)//�������240us���ǵ͵�ƽ ����ds18b20û�����ݻ��� ��ʧ������
			return; 
	}
	
}

//��Ƭ����DS18B20дһλ  0  
void Write_Bit_0(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_7); //����
	Delay_us(90);  //��60us --- 120us֮��
	GPIO_SetBits(GPIOB,GPIO_Pin_7);  //����
	Delay_us(10);  //���ߴ���1us ����ѡ��10us
}
//��Ƭ����DS18B20дһλ 1
void Write_Bit_1(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_7); //���� 
	Delay_us(10);  //���ʹ���1us
	GPIO_SetBits(GPIOB,GPIO_Pin_7);  //����
	Delay_us(90); //�ҵ������ ���ߴ���60us����д1 ����ѡ��90us
}
 
//��Ƭ����DS18B20��һλ 
unsigned char Read_Bit(void)
{
	DS_DIR_OUT(); //PB9Ϊ�������ģʽ
	GPIO_ResetBits(GPIOB,GPIO_Pin_7);  //����
	Delay_us(10); //����1us ѡ��10us
	DS_DIR_IN(); //PB9Ϊ��������ģʽ
	Delay_us(10); //�ȴ�һ�� ���ں����жϸùܽŷ��ص��Ǹߵ�ƽ���ǵ͵�ƽ ̫�󲻺��Թ�80us���ص����ݾ������
	if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7) == SET)
	{
		return 1; //�ߵ�ƽ����1 
	}
	else
	{
		return 0; //�͵�ƽ����0
	}
}

//��һ���ֽ�
unsigned char ReadOneChar(void)  			
{
	unsigned char i=0; 		
	unsigned char dat=0; 
	for (i=0;i<8;i++) 		
	{
		dat = dat | (Read_Bit() << i); //DS18B20���ֲ����ǵ�λ��ʼ��������ģ�Ҫ��ȡһ���������ֽڣ������ӾͿ��ԡ�
	} 
	return dat;
}
 
//дһ���ֽ�
void WriteOneChar(unsigned char dat) //datҪ���͵�����
{ 
	unsigned char i=0; 		
	DS_DIR_OUT(); //�������
	for(i=8;i>0;i--) 		//��15~60us֮��������߽��в���������Ǹߵ�ƽ��д1����д0������ 
	{
		if((dat & 0x01) == 1)
		{
			Write_Bit_1();
		}
		else
		{
			Write_Bit_0();
		}
		dat >>= 1;
	} 
}

//���¶�ֵ����λ��tempL;��λ��tempH;��
int ReadTemperature(void) 
{ 
	Init_DS18B20(); 					//��ʼ��
	WriteOneChar(0xcc); 				//���������кŵĲ���
	WriteOneChar(0x44); 				//�����¶�ת��
	Delay_us(1000);					    //ת����Ҫһ��ʱ�䣬��ʱ 
	Init_DS18B20(); 					//��ʼ��
	WriteOneChar(0xcc); 				//���������кŵĲ��� 
	WriteOneChar(0xbe); 				//���¶ȼĴ�����ͷ����ֵ�ֱ�Ϊ�¶ȵĵ�λ�͸�λ��	
	tempL=ReadOneChar(); 				//�����¶ȵĵ�λLSB
	tempH=ReadOneChar(); 				//�����¶ȵĸ�λMSB	
	if(tempH>0x7f)      				//���λΪ1ʱ�¶��Ǹ�
	{
		tempL=~tempL;					//����ת����ȡ����һ
		tempH=~tempH+1;       
		fg=0;      						//��ȡ�¶�Ϊ��ʱfg=0
	}
	sdata = (tempH << 8) + tempL;
	sdata = (sdata * 0.0625)*100;  //�����100 ���ڱ���һλС���ˣ���Ϊ����unsigned int���Ͳ���float��
	return sdata;
}


