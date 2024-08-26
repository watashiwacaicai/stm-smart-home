#include "stm32f10x.h"                  // Device header

void ADH_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//adcʱ�ӷ�Ƶ����Ϊadc���ֻ��14mhz
	
	GPIO_InitTypeDef llp;//���ò�������
	llp.GPIO_Mode = GPIO_Mode_AIN;//ģ������
	llp.GPIO_Pin = GPIO_Pin_1;
	llp.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &llp);
	
	
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5);//���ù����飬���һ���ǲ���ʱ�䣬�����ڶ����ǲ����������
	
	ADC_InitTypeDef ADC_Initoop;//��ʼ��adc
	ADC_Initoop.ADC_Mode = ADC_Mode_Independent;//��Ϊ����ģʽ
	ADC_Initoop.ADC_DataAlign = ADC_DataAlign_Right;//���ݶ��뷽ʽ�������
	ADC_Initoop.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//�ⲿ������ʽ
	ADC_Initoop.ADC_ContinuousConvMode = DISABLE;//����ת��ģʽ
	ADC_Initoop.ADC_ScanConvMode = DISABLE;//�Ƿ�ɨ��ģʽ
	ADC_Initoop.ADC_NbrOfChannel = 1;//ɨ��ģʽ�µ�ͨ��ѡ��
	ADC_Init(ADC1, &ADC_Initoop);
	
	ADC_Cmd(ADC1, ENABLE);//����adc
	
	ADC_ResetCalibration(ADC1);//�˴�ΪadcУ׼
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1) == SET);
}

uint16_t AD_GetValue(void)
{
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//�������ת��
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);//�ȴ�ת�����
	return ADC_GetConversionValue(ADC1);//��ֵ
}
