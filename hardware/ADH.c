#include "stm32f10x.h"                  // Device header

void ADH_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//adc时钟分频，因为adc最大只能14mhz
	
	GPIO_InitTypeDef llp;//配置采样引脚
	llp.GPIO_Mode = GPIO_Mode_AIN;//模拟输入
	llp.GPIO_Pin = GPIO_Pin_1;
	llp.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &llp);
	
	
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5);//配置规则组，最后一个是采样时间，倒数第二个是采样组的序列
	
	ADC_InitTypeDef ADC_Initoop;//初始化adc
	ADC_Initoop.ADC_Mode = ADC_Mode_Independent;//此为独立模式
	ADC_Initoop.ADC_DataAlign = ADC_DataAlign_Right;//数据对齐方式，左和右
	ADC_Initoop.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//外部触发方式
	ADC_Initoop.ADC_ContinuousConvMode = DISABLE;//连续转换模式
	ADC_Initoop.ADC_ScanConvMode = DISABLE;//是否扫描模式
	ADC_Initoop.ADC_NbrOfChannel = 1;//扫描模式下的通道选择
	ADC_Init(ADC1, &ADC_Initoop);
	
	ADC_Cmd(ADC1, ENABLE);//开启adc
	
	ADC_ResetCalibration(ADC1);//此处为adc校准
	while (ADC_GetResetCalibrationStatus(ADC1) == SET);
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1) == SET);
}

uint16_t AD_GetValue(void)
{
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//软件触发转换
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);//等待转换完成
	return ADC_GetConversionValue(ADC1);//读值
}
