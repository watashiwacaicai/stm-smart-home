#include "stm32f10x.h"                  // Device header

void Timer_Init(void){
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//使能对应需要的时钟
	TIM_InternalClockConfig(TIM2);//选择为内部时钟
	
	TIM_TimeBaseInitTypeDef ppo;
	ppo.TIM_ClockDivision=TIM_CKD_DIV1;//防抖动的输出滤波，1不分频，2为2分频，4为4分频
	ppo.TIM_CounterMode=TIM_CounterMode_Up;//计数器的计数模式，从小到大，从大到小，小到大再到小，即向上，向下计数以及中央对齐
	ppo.TIM_Period=10000-1;       //此定义下计数一秒，两参数取值范围均为65535；period：总共要计到的数；
	ppo.TIM_Prescaler=7200-1;//pres：计数的频率；例：上面或下面的数缩小10倍均导致计数由1秒变为0.1秒
	ppo.TIM_RepetitionCounter=0;//重复定时器，仅高级定时器可用
	TIM_TimeBaseInit(TIM2,&ppo);
	
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);//防止复位后直接进入一次中断，编写位置固定
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//使能定时中断
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断分组
	
	NVIC_InitTypeDef ffg;  //初始化中断
	ffg.NVIC_IRQChannel=TIM2_IRQn;//选择nvic通道，有些接口共用一个通道
	ffg.NVIC_IRQChannelCmd=ENABLE;
	ffg.NVIC_IRQChannelPreemptionPriority=2;
	ffg.NVIC_IRQChannelSubPriority=2;
	NVIC_Init(&ffg);
	
	TIM_Cmd(TIM2,ENABLE);//启动定时器
}


	
