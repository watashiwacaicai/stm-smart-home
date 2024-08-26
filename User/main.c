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
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//开启引脚时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//开启afio时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);//开时钟
	
	GPIO_InitTypeDef GPIO_InitStructure;//单片机运行指示灯
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
	
	GPIO_InitTypeDef ssd;//初始化风扇自动指示脚
	ssd.GPIO_Mode = GPIO_Mode_Out_PP;
	ssd.GPIO_Pin = GPIO_Pin_2;
	ssd.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &ssd);
	
	GPIO_InitTypeDef ssg;//初始化窗帘自动指示脚，小灯自动指示脚，窗帘开关指示脚
	ssg.GPIO_Mode = GPIO_Mode_Out_PP;
	ssg.GPIO_Pin = GPIO_Pin_11;//11做窗帘自动
	ssg.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &ssg);
	ssg.GPIO_Pin = GPIO_Pin_12;//12做小灯自动
	GPIO_Init(GPIOA, &ssg);
	ssg.GPIO_Pin = GPIO_Pin_5;//5做窗帘开关
	GPIO_Init(GPIOB, &ssg);
	
	GPIO_InitTypeDef edd;//配置LED小灯引脚
	edd.GPIO_Mode = GPIO_Mode_Out_PP;
	edd.GPIO_Pin = GPIO_Pin_10;
	edd.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &edd);
	
	GPIO_InitTypeDef qqq;//配置红外读取引脚
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
	
	USART_SendString(USART1,"<M>0");//关闭背景音乐
	
	
	while(1){
		
		if(people>2){//上电无反应
		
		}
		
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==RESET){//读取电平
		Delay_us(25);
			if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==RESET){
			people=!people;
			while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)==RESET);
		}
		}
		
		if(people==0){ //人回来时
				
			if(voice==1){
			showCN();//显示汉字
	  showtemlight();//显示光照与温度
				USART_SendString(USART1,"<G>欢迎回来");
			voice=0;
				back();
				delay_mss(1000);
				OLED_ShowString(4,1,"             ");//清第四行
			fan_switch=1;//打开风扇
			}
		
			if(fan_auto==1){  //风扇的自动模式调档			
			if(wendu>=24&&wendu<27){
			fan_dang=1;
			}else if(wendu>=27&&wendu<30){
			fan_dang=2;
			}else if(wendu>=30){
			fan_dang=3;
			}
		}
		
		if(fan_auto==1&&fan_switch==1){//风扇的自动指示
		GPIO_SetBits(GPIOA,GPIO_Pin_2);
		}else{
		GPIO_ResetBits(GPIOA,GPIO_Pin_2);
		}
		if(light_auto==1){//小灯的自动指示
		GPIO_SetBits(GPIOA,GPIO_Pin_12);
		}else{
		GPIO_ResetBits(GPIOA,GPIO_Pin_12);
		}
		if(certain_auto==1){//窗帘的自动指示
		GPIO_SetBits(GPIOA,GPIO_Pin_11);
		}else{
		GPIO_ResetBits(GPIOA,GPIO_Pin_11);
		}
		if(certain_switch==1){//窗帘的开关指示
		GPIO_SetBits(GPIOB,GPIO_Pin_5);
		}else{
		GPIO_ResetBits(GPIOB,GPIO_Pin_5);
		}
		
		
		if(fan_switch==1){    //风扇档数自调
		switch(fan_dang){
			case 1:	GPIO_SetBits(GPIOA,GPIO_Pin_3);GPIO_ResetBits(GPIOA,GPIO_Pin_4);TIM_SetCompare1(TIM3, 80);break;	
			case 2: GPIO_SetBits(GPIOA,GPIO_Pin_3);GPIO_ResetBits(GPIOA,GPIO_Pin_4);TIM_SetCompare1(TIM3, 50);break;
			case 3: GPIO_SetBits(GPIOA,GPIO_Pin_3);GPIO_ResetBits(GPIOA,GPIO_Pin_4);TIM_SetCompare1(TIM3, 15);break;}
		}else{
		GPIO_ResetBits(GPIOA,GPIO_Pin_3);GPIO_ResetBits(GPIOA,GPIO_Pin_4);
		}
		
		if(certain_auto==1&&certain_switch!=1){  //窗帘的自动打开
		GPIO_SetBits(GPIOB,GPIO_Pin_0);//此向做开
	  GPIO_ResetBits(GPIOB,GPIO_Pin_1);
	  TIM_SetCompare2(TIM3, 66);//对通道2的操作	
	  delay_mss(2000);
	  GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	  GPIO_ResetBits(GPIOB,GPIO_Pin_1);
		certain_switch=1;
		}
		
		if(light_auto==1){		//自动模式的灯
			if(liangdu<1200){
			GPIO_SetBits(GPIOA,GPIO_Pin_10);light_switch=1;//开灯
			}else if(liangdu>1200){
			GPIO_ResetBits(GPIOA,GPIO_Pin_10);light_switch=0;//关灯
			}
		}else{    //手动模式的灯
		switch(light_switch){
			case 1:GPIO_SetBits(GPIOA,GPIO_Pin_10);light_switch=1;break;//开灯
			case 0:GPIO_ResetBits(GPIOA,GPIO_Pin_10);light_switch=0;break;//关灯
		}
		}
		
	}
		
	
	if(people==1){  //人离开时
	
		if(voice==0){
			USART_SendString(USART1,"<G>一路顺风");
			voice=1;
				go();
				delay_mss(1000);
				OLED_Clear();//清屏
			}
	
	if(certain_auto==1&&certain_switch!=0){  //窗帘的自动关闭
		GPIO_ResetBits(GPIOB,GPIO_Pin_0);//此向做关
	  GPIO_SetBits(GPIOB,GPIO_Pin_1);		
	  TIM_SetCompare2(TIM3, 66);//对通道2的操作	
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
	fan_auto=1; //各自恢复自动模式
	fan_switch=0;
	fan_dang=1;//风扇回档
	certain_switch=0;
	certain_auto=1;
	light_auto=1;	
	light_switch=0;
		
	}
	
}
}

void EXTI15_10_IRQHandler(void){  //风扇与led灯的中断服务函数
	if(people==0){//仅有人时有效
if(EXTI_GetITStatus(EXTI_Line14)==SET){//判断是否是14脚触发的中断通道,风扇开关
	Delay_us(25);
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)==RESET);
	fan_switch=!fan_switch;
	
	EXTI_ClearITPendingBit(EXTI_Line14);//清除14脚中断的标志位
}
if((EXTI_GetITStatus(EXTI_Line13)==SET)&&fan_switch==1){//判断是否是13脚触发的中断通道，风扇自动
	Delay_us(25);
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)==RESET);
	fan_auto=!fan_auto;
	
	EXTI_ClearITPendingBit(EXTI_Line13);//清除13脚中断的标志位
}
if((EXTI_GetITStatus(EXTI_Line12)==SET)&&fan_switch==1){//判断是否是12脚触发的中断通道，升档
	Delay_us(25);
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)==RESET);
	if(fan_dang==3){
	fan_dang=fan_dang;
	}else{
	fan_dang++;fan_auto=0;
	}	
	EXTI_ClearITPendingBit(EXTI_Line12);//清除12脚中断的标志位
}
if((EXTI_GetITStatus(EXTI_Line11)==SET)&&fan_switch==1){//判断是否是11脚触发的中断通道，降档
	Delay_us(25);
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==RESET);
	if(fan_dang==1){
	fan_dang=fan_dang;
	}else{
	fan_dang--;fan_auto=0;
	}	
	EXTI_ClearITPendingBit(EXTI_Line11);//清除11脚中断的标志位
}


if(EXTI_GetITStatus(EXTI_Line10)==SET){//判断是否是10脚触发的中断通道,灯开关
	Delay_us(25);
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)==RESET);
	light_switch=!light_switch;
	light_auto=0;
	EXTI_ClearITPendingBit(EXTI_Line10);//清除10脚中断的标志位
}
if(EXTI_GetITStatus(EXTI_Line15)==SET){//判断是否是15脚触发的中断通道,灯自动
	Delay_us(25);
	while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==RESET);
	light_auto=!light_auto;
	
	EXTI_ClearITPendingBit(EXTI_Line15);//清除14脚中断的标志位
}
}
}


void EXTI9_5_IRQHandler(void){  //窗帘的中断服务函数
if(people==0){
if(EXTI_GetITStatus(EXTI_Line5)==SET){//判断是否是5脚触发的中断通道,窗帘自动
	Delay_us(25);
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)==RESET);
	certain_auto=!certain_auto;
	
	EXTI_ClearITPendingBit(EXTI_Line5);//清除5脚中断的标志位
}

if(EXTI_GetITStatus(EXTI_Line8)==SET){//判断是否是8脚触发的中断通道,窗帘开
	
	Delay_us(25);
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)==RESET);
	
	if(certain_switch!=1){
	GPIO_SetBits(GPIOB,GPIO_Pin_0);//此向做开
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);
	TIM_SetCompare2(TIM3, 66);//对通道2的操作	
	delay_mss(2000);
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);
		certain_switch=1;
		certain_auto=0;
	}
	OLED_Clear();
	EXTI_ClearITPendingBit(EXTI_Line8);//清除8脚中断的标志位

}

if(EXTI_GetITStatus(EXTI_Line9)==SET){//判断是否是9脚触发的中断通道,窗帘关
	
	Delay_us(25);
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_9)==RESET);
 
	if(certain_switch!=0){
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);//此向做关
	GPIO_SetBits(GPIOB,GPIO_Pin_1);	
	TIM_SetCompare2(TIM3, 66);//对通道2的操作	
	delay_mss(2000);
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);
		certain_switch=0;
		certain_auto=0;		
}
	OLED_Clear();
EXTI_ClearITPendingBit(EXTI_Line9);//清除9脚中断的标志位

}

}
}

void TIM2_IRQHandler(void){//定时器中断函数
if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET){//检测标志位
	
	wendu=ReadTemperature();
	liangdu=AD_GetValue();
	aft=(wendu/10)%10;
	pre=wendu/100;
	wendu=wendu/100;
	liangdu=3800-liangdu;
	
	if(people==0){
	showtemlight();//显示光照与温度
	showCN();//显示汉字
	}
TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//清除标志位
}
}

void showCN(void){
  OLED_ShowChinese(1,1,13);//智
	OLED_ShowChinese(1,2,14);//能
	OLED_ShowChinese(1,3,15);//家
	OLED_ShowChinese(1,4,16);//居
	OLED_ShowChinese(1,5,17);//系
	OLED_ShowChinese(1,6,18);//统
	OLED_ShowChinese(2,1,11);//室
	OLED_ShowChinese(2,2,12);//内
	OLED_ShowChinese(2,3,0);//温
	OLED_ShowChinese(2,4,1);//度
	OLED_ShowChar(2,9,':');
	OLED_ShowChinese(3,1,11);//室
	OLED_ShowChinese(3,2,12);//内
	OLED_ShowChinese(3,3,19);//光
	OLED_ShowChinese(3,4,20);//照
	OLED_ShowChar(3,9,':');
}

void showtemlight(void){
	OLED_ShowNum(2,10,pre,2);
	OLED_ShowChar(2,12,'.');
	OLED_ShowNum(2,13,aft,1);
	OLED_ShowNum(3,10,liangdu,4);
}

void back(void){
  OLED_ShowChinese(4,1,3);//欢
	OLED_ShowChinese(4,2,4);//迎
	OLED_ShowChinese(4,3,5);//回
	OLED_ShowChinese(4,4,6);//来
	OLED_ShowChar(4,9,'!');
	
}

void go(void){
  OLED_ShowChinese(4,1,7);//一
	OLED_ShowChinese(4,2,8);//路
	OLED_ShowChinese(4,3,9);//顺
	OLED_ShowChinese(4,4,10);//风
	OLED_ShowChar(4,9,'!');
	
}
