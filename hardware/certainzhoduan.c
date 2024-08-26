#include "stm32f10x.h"                 

void certainzhoduan_Init(void){
	
	GPIO_InitTypeDef ccb;
	ccb.GPIO_Mode=GPIO_Mode_IPU;
	ccb.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9;
	ccb.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&ccb);	
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource5);//afio���ߵ�exti
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource8);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource9);
	
	
	EXTI_InitTypeDef ssdk;
	ssdk.EXTI_Line=EXTI_Line5;//����
	ssdk.EXTI_LineCmd=ENABLE;//�����ж�
	ssdk.EXTI_Mode=EXTI_Mode_Interrupt;//���ж�ģʽ���¼�ģʽ��ѡ
	ssdk.EXTI_Trigger=EXTI_Trigger_Rising;//ѡ����صĴ�����ʽ���������½���������
	EXTI_Init(&ssdk);
	ssdk.EXTI_Line=EXTI_Line8;
		EXTI_Init(&ssdk);
	ssdk.EXTI_Line=EXTI_Line9;
		EXTI_Init(&ssdk);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);//�жϷ���
	NVIC_InitTypeDef jjh;
	jjh.NVIC_IRQChannel=EXTI9_5_IRQn;//ѡ��nvicͨ������Щ�ӿڹ���һ��ͨ��
	jjh.NVIC_IRQChannelCmd=ENABLE;//�Ƿ�ʹ��ͨ��
	jjh.NVIC_IRQChannelPreemptionPriority=0;//��ռ���ȼ�
	jjh.NVIC_IRQChannelSubPriority=5;//�����ж�Ƕ�׵���Ӧ���ȼ�
	NVIC_Init(&jjh);
	//�жϺ���ѡ����鿴�����ļ�100�����ң������ж�ͨ����ѡ��
}

