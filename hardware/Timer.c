#include "stm32f10x.h"                  // Device header

void Timer_Init(void){
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//ʹ�ܶ�Ӧ��Ҫ��ʱ��
	TIM_InternalClockConfig(TIM2);//ѡ��Ϊ�ڲ�ʱ��
	
	TIM_TimeBaseInitTypeDef ppo;
	ppo.TIM_ClockDivision=TIM_CKD_DIV1;//������������˲���1����Ƶ��2Ϊ2��Ƶ��4Ϊ4��Ƶ
	ppo.TIM_CounterMode=TIM_CounterMode_Up;//�������ļ���ģʽ����С���󣬴Ӵ�С��С�����ٵ�С�������ϣ����¼����Լ��������
	ppo.TIM_Period=10000-1;       //�˶����¼���һ�룬������ȡֵ��Χ��Ϊ65535��period���ܹ�Ҫ�Ƶ�������
	ppo.TIM_Prescaler=7200-1;//pres��������Ƶ�ʣ�������������������С10�������¼�����1���Ϊ0.1��
	ppo.TIM_RepetitionCounter=0;//�ظ���ʱ�������߼���ʱ������
	TIM_TimeBaseInit(TIM2,&ppo);
	
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);//��ֹ��λ��ֱ�ӽ���һ���жϣ���дλ�ù̶�
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//ʹ�ܶ�ʱ�ж�
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�жϷ���
	
	NVIC_InitTypeDef ffg;  //��ʼ���ж�
	ffg.NVIC_IRQChannel=TIM2_IRQn;//ѡ��nvicͨ������Щ�ӿڹ���һ��ͨ��
	ffg.NVIC_IRQChannelCmd=ENABLE;
	ffg.NVIC_IRQChannelPreemptionPriority=2;
	ffg.NVIC_IRQChannelSubPriority=2;
	NVIC_Init(&ffg);
	
	TIM_Cmd(TIM2,ENABLE);//������ʱ��
}


	
