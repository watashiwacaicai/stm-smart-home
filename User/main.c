#include "stm32f10x.h"                  
#include "Delay.h"
#include "Fanzhoduan.h"
#include "PWM.h"
#include "certainzhoduan.h"
#include "OLED.h"
#include "Timer.h"
#include "Dsb.h"
#include "ADH.h"
#include "chuankou.h"

unsigned int wendu;
uint8_t fan_switch=1,fan_auto=1,fan_dang=1,light_auto=1,light_switch=1,certain_switch=0,certain_auto=1,people=5,voice=1,oled=0;
uint16_t liangdu=0;
int pre=0,aft=0;
 
void showCN(void);
void showtemlight(void);
void TIM2_IRQHandler(void);
void back(void);
void go(void);

int main(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//��������ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//����afioʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);//��ʱ��
	
	GPIO_InitTypeDef GPIO_InitStructure;//��Ƭ������ָʾ��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
	
	GPIO_InitTypeDef ssd;//��ʼ�������Զ�ָʾ��
	ssd.GPIO_Mode = GPIO_Mode_Out_PP;
	ssd.GPIO_Pin = GPIO_Pin_2;
	ssd.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &ssd);
	
	GPIO_InitTypeDef ssg;//��ʼ�������Զ�ָʾ�ţ�С���Զ�ָʾ�ţ���������ָʾ��
	ssg.GPIO_Mode = GPIO_Mode_Out_PP;
	ssg.GPIO_Pin = GPIO_Pin_11;//11�������Զ�
	ssg.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &ssg);
	ssg.GPIO_Pin = GPIO_Pin_12;//12��С���Զ�
	GPIO_Init(GPIOA, &ssg);
	ssg.GPIO_Pin = GPIO_Pin_5;//5����������
	GPIO_Init(GPIOB, &ssg);
	
	GPIO_InitTypeDef edd;//����LEDС������
	edd.GPIO_Mode = GPIO_Mode_Out_PP;
	edd.GPIO_Pin = GPIO_Pin_10;
	edd.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &edd);
	
	GPIO_InitTypeDef qqq;//���ú����ȡ����
	qqq.GPIO_Mode = GPIO_Mode_IPU;
	qqq.GPIO_Pin = GPIO_Pin_0;
	qqq.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &qqq);
	
	zhoduan_Init();
	certainzhoduan_Init();
	pwmk_init();
	OLED_Init();
	ADH_Init();
	Timer_Init();
	chuankou_init();
	
	USART_SendString(USART1,"<M>0");//�رձ�������
	
	
	while(1){
		
		if(people>2){//�ϵ��޷�Ӧ
		
		}
		
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==RESET){//��ȡ��ƽ
		Delay_us(25);
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==RESET){
			people=!people;
			while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==RESET);
		}
		}
		
		if(people==0){ //�˻���ʱ
				
			if(voice==1){
			showCN();//��ʾ����
	  showtemlight();//��ʾ�������¶�
				USART_SendString(USART1,"<G>��ӭ����");
			voice=0;
				back();
				delay_mss(1000);
				OLED_ShowString(4,1,"             ");//�������
			fan_switch=1;//�򿪷���
			}
		
			if(fan_auto==1){  //���ȵ��Զ�ģʽ����			
			if(wendu>=24&&wendu<27){
			fan_dang=1;
			}else if(wendu>=27&&wendu<30){
			fan_dang=2;
			}else if(wendu>=30){
			fan_dang=3;
			}
		}
		
		if(fan_auto==1&&fan_switch==1){//���ȵ��Զ�ָʾ
		GPIO_SetBits(GPIOA,GPIO_Pin_2);
		}else{
		GPIO_ResetBits(GPIOA,GPIO_Pin_2);
		}
		if(light_auto==1){//С�Ƶ��Զ�ָʾ
		GPIO_SetBits(GPIOA,GPIO_Pin_12);
		}else{
		GPIO_ResetBits(GPIOA,GPIO_Pin_12);
		}
		if(certain_auto==1){//�������Զ�ָʾ
		GPIO_SetBits(GPIOA,GPIO_Pin_11);
		}else{
		GPIO_ResetBits(GPIOA,GPIO_Pin_11);
		}
		if(certain_switch==1){//�����Ŀ���ָʾ
		GPIO_SetBits(GPIOB,GPIO_Pin_5);
		}else{
		GPIO_ResetBits(GPIOB,GPIO_Pin_5);
		}
		
		
		if(fan_switch==1){    //���ȵ����Ե�
		switch(fan_dang){
			case 1:	GPIO_SetBits(GPIOA,GPIO_Pin_3);GPIO_ResetBits(GPIOA,GPIO_Pin_4);TIM_SetCompare1(TIM3, 80);break;	
			case 2: GPIO_SetBits(GPIOA,GPIO_Pin_3);GPIO_ResetBits(GPIOA,GPIO_Pin_4);TIM_SetCompare1(TIM3, 50);break;
			case 3: GPIO_SetBits(GPIOA,GPIO_Pin_3);GPIO_ResetBits(GPIOA,GPIO_Pin_4);TIM_SetCompare1(TIM3, 15);break;}
		}else{
		GPIO_ResetBits(GPIOA,GPIO_Pin_3);GPIO_ResetBits(GPIOA,GPIO_Pin_4);
		}
		
		if(certain_auto==1&&certain_switch!=1){  //�������Զ���
		GPIO_SetBits(GPIOB,GPIO_Pin_0);//��������
	  GPIO_ResetBits(GPIOB,GPIO_Pin_1);
	  TIM_SetCompare2(TIM3, 66);//��ͨ��2�Ĳ���	
	  delay_mss(2000);
	  GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	  GPIO_ResetBits(GPIOB,GPIO_Pin_1);
		certain_switch=1;
		}
		
		if(light_auto==1){		//�Զ�ģʽ�ĵ�
			if(liangdu<1200){
			GPIO_SetBits(GPIOA,GPIO_Pin_10);light_switch=1;//����
			}else if(liangdu>1200){
			GPIO_ResetBits(GPIOA,GPIO_Pin_10);light_switch=0;//�ص�
			}
		}else{    //�ֶ�ģʽ�ĵ�
		switch(light_switch){
			case 1:GPIO_SetBits(GPIOA,GPIO_Pin_10);light_switch=1;break;//����
			case 0:GPIO_ResetBits(GPIOA,GPIO_Pin_10);light_switch=0;break;//�ص�
		}
		}
		
	}
		
	
	if(people==1){  //���뿪ʱ
	
		if(voice==0){
			USART_SendString(USART1,"<G>һ·˳��");
			voice=1;
				go();
				delay_mss(1000);
				OLED_Clear();//����
			}
	
	if(certain_auto==1&&certain_switch!=0){  //�������Զ��ر�
		GPIO_ResetBits(GPIOB,GPIO_Pin_0);//��������
	  GPIO_SetBits(GPIOB,GPIO_Pin_1);		
	  TIM_SetCompare2(TIM3, 66);//��ͨ��2�Ĳ���	
	  delay_mss(2000);
	  GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	  GPIO_ResetBits(GPIOB,GPIO_Pin_1);
		certain_switch=0;
		}
	GPIO_ResetBits(GPIOA,GPIO_Pin_10);
	GPIO_ResetBits(GPIOA,GPIO_Pin_2);
	GPIO_ResetBits(GPIOA,GPIO_Pin_11);
	GPIO_ResetBits(GPIOA,GPIO_Pin_12);
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	GPIO_ResetBits(GPIOA,GPIO_Pin_3);
	GPIO_ResetBits(GPIOB,GPIO_Pin_5);
	fan_auto=1; //���Իָ��Զ�ģʽ
	fan_switch=0;
	fan_dang=1;//���Ȼص�
	certain_switch=0;
	certain_auto=1;
	light_auto=1;	
	light_switch=0;
		
	}
	
}
}

void EXTI15_10_IRQHandler(void){  //������led�Ƶ��жϷ�����
	if(people==0){//������ʱ��Ч
if(EXTI_GetITStatus(EXTI_Line14)==SET){//�ж��Ƿ���14�Ŵ������ж�ͨ��,���ȿ���
	Delay_us(25);
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)==RESET);
	fan_switch=!fan_switch;
	
	EXTI_ClearITPendingBit(EXTI_Line14);//���14���жϵı�־λ
}
if((EXTI_GetITStatus(EXTI_Line13)==SET)&&fan_switch==1){//�ж��Ƿ���13�Ŵ������ж�ͨ���������Զ�
	Delay_us(25);
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)==RESET);
	fan_auto=!fan_auto;
	
	EXTI_ClearITPendingBit(EXTI_Line13);//���13���жϵı�־λ
}
if((EXTI_GetITStatus(EXTI_Line12)==SET)&&fan_switch==1){//�ж��Ƿ���12�Ŵ������ж�ͨ��������
	Delay_us(25);
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)==RESET);
	if(fan_dang==3){
	fan_dang=fan_dang;
	}else{
	fan_dang++;fan_auto=0;
	}	
	EXTI_ClearITPendingBit(EXTI_Line12);//���12���жϵı�־λ
}
if((EXTI_GetITStatus(EXTI_Line11)==SET)&&fan_switch==1){//�ж��Ƿ���11�Ŵ������ж�ͨ��������
	Delay_us(25);
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==RESET);
	if(fan_dang==1){
	fan_dang=fan_dang;
	}else{
	fan_dang--;fan_auto=0;
	}	
	EXTI_ClearITPendingBit(EXTI_Line11);//���11���жϵı�־λ
}


if(EXTI_GetITStatus(EXTI_Line10)==SET){//�ж��Ƿ���10�Ŵ������ж�ͨ��,�ƿ���
	Delay_us(25);
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)==RESET);
	light_switch=!light_switch;
	light_auto=0;
	EXTI_ClearITPendingBit(EXTI_Line10);//���10���жϵı�־λ
}
if(EXTI_GetITStatus(EXTI_Line15)==SET){//�ж��Ƿ���15�Ŵ������ж�ͨ��,���Զ�
	Delay_us(25);
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==RESET);
	light_auto=!light_auto;
	
	EXTI_ClearITPendingBit(EXTI_Line15);//���14���жϵı�־λ
}
}
}


void EXTI9_5_IRQHandler(void){  //�������жϷ�����
if(people==0){
if(EXTI_GetITStatus(EXTI_Line5)==SET){//�ж��Ƿ���5�Ŵ������ж�ͨ��,�����Զ�
	Delay_us(25);
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==RESET);
	certain_auto=!certain_auto;
	
	EXTI_ClearITPendingBit(EXTI_Line5);//���5���жϵı�־λ
}

if(EXTI_GetITStatus(EXTI_Line8)==SET){//�ж��Ƿ���8�Ŵ������ж�ͨ��,������
	
	Delay_us(25);
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)==RESET);
	
	if(certain_switch!=1){
	GPIO_SetBits(GPIOB,GPIO_Pin_0);//��������
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);
	TIM_SetCompare2(TIM3, 66);//��ͨ��2�Ĳ���	
	delay_mss(2000);
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);
		certain_switch=1;
		certain_auto=0;
	}
	OLED_Clear();
	EXTI_ClearITPendingBit(EXTI_Line8);//���8���жϵı�־λ

}

if(EXTI_GetITStatus(EXTI_Line9)==SET){//�ж��Ƿ���9�Ŵ������ж�ͨ��,������
	
	Delay_us(25);
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_9)==RESET);
 
	if(certain_switch!=0){
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);//��������
	GPIO_SetBits(GPIOB,GPIO_Pin_1);	
	TIM_SetCompare2(TIM3, 66);//��ͨ��2�Ĳ���	
	delay_mss(2000);
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);
		certain_switch=0;
		certain_auto=0;		
}
	OLED_Clear();
EXTI_ClearITPendingBit(EXTI_Line9);//���9���жϵı�־λ

}

}
}

void TIM2_IRQHandler(void){//��ʱ���жϺ���
if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET){//����־λ
	
	wendu=ReadTemperature();
	liangdu=AD_GetValue();
	aft=(wendu/10)%10;
	pre=wendu/100;
	wendu=wendu/100;
	liangdu=3800-liangdu;
	
	if(people==0){
	showtemlight();//��ʾ�������¶�
	showCN();//��ʾ����
	}
TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//�����־λ
}
}

void showCN(void){
  OLED_ShowChinese(1,1,13);//��
	OLED_ShowChinese(1,2,14);//��
	OLED_ShowChinese(1,3,15);//��
	OLED_ShowChinese(1,4,16);//��
	OLED_ShowChinese(1,5,17);//ϵ
	OLED_ShowChinese(1,6,18);//ͳ
	OLED_ShowChinese(2,1,11);//��
	OLED_ShowChinese(2,2,12);//��
	OLED_ShowChinese(2,3,0);//��
	OLED_ShowChinese(2,4,1);//��
	OLED_ShowChar(2,9,':');
	OLED_ShowChinese(3,1,11);//��
	OLED_ShowChinese(3,2,12);//��
	OLED_ShowChinese(3,3,19);//��
	OLED_ShowChinese(3,4,20);//��
	OLED_ShowChar(3,9,':');
}

void showtemlight(void){
	OLED_ShowNum(2,10,pre,2);
	OLED_ShowChar(2,12,'.');
	OLED_ShowNum(2,13,aft,1);
	OLED_ShowNum(3,10,liangdu,4);
}

void back(void){
  OLED_ShowChinese(4,1,3);//��
	OLED_ShowChinese(4,2,4);//ӭ
	OLED_ShowChinese(4,3,5);//��
	OLED_ShowChinese(4,4,6);//��
	OLED_ShowChar(4,9,'!');
	
}

void go(void){
  OLED_ShowChinese(4,1,7);//һ
	OLED_ShowChinese(4,2,8);//·
	OLED_ShowChinese(4,3,9);//˳
	OLED_ShowChinese(4,4,10);//��
	OLED_ShowChar(4,9,'!');
	
}
