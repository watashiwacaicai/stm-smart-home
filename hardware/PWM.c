#include "stm32f10x.h"                  

void pwmk_init(void){

	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);//开时钟
	
	GPIO_InitTypeDef jdd;//初始化风扇控制脚
	jdd.GPIO_Mode = GPIO_Mode_Out_PP;
	jdd.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;
	jdd.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &jdd);
	
	
	GPIO_InitTypeDef tty;//初始化窗帘控制脚
	tty.GPIO_Mode = GPIO_Mode_Out_PP;
	tty.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	tty.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &tty);
	
	GPIO_InitTypeDef jk;//pwm脚,6做风扇7做窗帘
	jk.GPIO_Mode = GPIO_Mode_AF_PP;
	jk.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	jk.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &jk);
	
	TIM_InternalClockConfig(TIM3);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;//配置时基单元
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;//配置输出比较
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;//模式2，高低反转，比较值越高越慢
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;		//CCR
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);//初始化通道1,风扇
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);//初始化通道2，窗帘
	
	TIM_Cmd(TIM3, ENABLE);

}
