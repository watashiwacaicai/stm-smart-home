#include "stm32f10x.h"                 

void certainzhoduan_Init(void){
	
	GPIO_InitTypeDef ccb;
	ccb.GPIO_Mode=GPIO_Mode_IPU;
	ccb.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9;
	ccb.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&ccb);	
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource5);//afio拉线到exti
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource8);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource9);
	
	
	EXTI_InitTypeDef ssdk;
	ssdk.EXTI_Line=EXTI_Line5;//架线
	ssdk.EXTI_LineCmd=ENABLE;//开启中断
	ssdk.EXTI_Mode=EXTI_Mode_Interrupt;//有中断模式和事件模式可选
	ssdk.EXTI_Trigger=EXTI_Trigger_Rising;//选择边沿的触发方式，上升，下降，或两个
	EXTI_Init(&ssdk);
	ssdk.EXTI_Line=EXTI_Line8;
		EXTI_Init(&ssdk);
	ssdk.EXTI_Line=EXTI_Line9;
		EXTI_Init(&ssdk);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);//中断分组
	NVIC_InitTypeDef jjh;
	jjh.NVIC_IRQChannel=EXTI9_5_IRQn;//选择nvic通道，有些接口共用一个通道
	jjh.NVIC_IRQChannelCmd=ENABLE;//是否使能通道
	jjh.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级
	jjh.NVIC_IRQChannelSubPriority=5;//用于中断嵌套的响应优先级
	NVIC_Init(&jjh);
	//中断函数选择请查看启动文件100行左右，按照中断通道来选择
}

