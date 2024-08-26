#include "stm32f10x.h"                  // Device header

void zhoduan_Init(void){
	
	GPIO_InitTypeDef ppk;
	ppk.GPIO_Mode=GPIO_Mode_IPU;
	ppk.GPIO_Pin=GPIO_Pin_14|GPIO_Pin_13|GPIO_Pin_12|GPIO_Pin_11|GPIO_Pin_10|GPIO_Pin_15;
	ppk.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&ppk);	
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource14);//afio拉线到exti
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource13);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource12);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource11);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource10);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource15);
	
	
	EXTI_InitTypeDef kkl;
	kkl.EXTI_Line=EXTI_Line14;//架线
	kkl.EXTI_LineCmd=ENABLE;//开启中断
	kkl.EXTI_Mode=EXTI_Mode_Interrupt;//有中断模式和事件模式可选
	kkl.EXTI_Trigger=EXTI_Trigger_Rising;//选择边沿的触发方式，上升，下降，或两个
	EXTI_Init(&kkl);
	kkl.EXTI_Line=EXTI_Line13;
		EXTI_Init(&kkl);
	kkl.EXTI_Line=EXTI_Line12;
		EXTI_Init(&kkl);
	kkl.EXTI_Line=EXTI_Line11;
		EXTI_Init(&kkl);
	kkl.EXTI_Line=EXTI_Line10;
		EXTI_Init(&kkl);
	kkl.EXTI_Line=EXTI_Line15;
		EXTI_Init(&kkl);
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//中断分组
	NVIC_InitTypeDef mml;
	mml.NVIC_IRQChannel=EXTI15_10_IRQn;//选择nvic通道，有些接口共用一个通道
	mml.NVIC_IRQChannelCmd=ENABLE;//是否使能通道
	mml.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级
	mml.NVIC_IRQChannelSubPriority=0;//用于中断嵌套的响应优先级
	NVIC_Init(&mml);
	//中断函数选择请查看启动文件100行左右，按照中断通道来选择
}


