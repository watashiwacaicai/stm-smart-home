#include "stm32f10x.h"                  // Device header

void zhoduan_Init(void){
	
	GPIO_InitTypeDef ppk;
	ppk.GPIO_Mode=GPIO_Mode_IPU;
	ppk.GPIO_Pin=GPIO_Pin_14|GPIO_Pin_13|GPIO_Pin_12|GPIO_Pin_11|GPIO_Pin_10|GPIO_Pin_15;
	ppk.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&ppk);	
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource14);//afio���ߵ�exti
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource13);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource12);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource11);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource10);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource15);
	
	
	EXTI_InitTypeDef kkl;
	kkl.EXTI_Line=EXTI_Line14;//����
	kkl.EXTI_LineCmd=ENABLE;//�����ж�
	kkl.EXTI_Mode=EXTI_Mode_Interrupt;//���ж�ģʽ���¼�ģʽ��ѡ
	kkl.EXTI_Trigger=EXTI_Trigger_Rising;//ѡ����صĴ�����ʽ���������½���������
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
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//�жϷ���
	NVIC_InitTypeDef mml;
	mml.NVIC_IRQChannel=EXTI15_10_IRQn;//ѡ��nvicͨ������Щ�ӿڹ���һ��ͨ��
	mml.NVIC_IRQChannelCmd=ENABLE;//�Ƿ�ʹ��ͨ��
	mml.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ�
	mml.NVIC_IRQChannelSubPriority=0;//�����ж�Ƕ�׵���Ӧ���ȼ�
	NVIC_Init(&mml);
	//�жϺ���ѡ����鿴�����ļ�100�����ң������ж�ͨ����ѡ��
}


