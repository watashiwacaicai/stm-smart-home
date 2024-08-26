#include "stm32f10x.h"                 
#include "Delay.h"

void sandstring(char*str){
uint8_t len;
	for(len=0;str[0]!='\0';len++){
		USART_SendData(USART1,str[len]);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	}
}

void USART_SendString(USART_TypeDef* USARTx, char *DataString)//更完整的发送函数
{
	int i = 0;
	USART_ClearFlag(USARTx,USART_FLAG_TC);//发送字符前清空标志位（否则缺失字符串的第一个字符）
	while(DataString[i] != '\0')		//字符串结束符
	{
		USART_SendData(USARTx,DataString[i]);	//每次发送字符串的一个字符
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TC) == 0);	//等待数据发送成功
		USART_ClearFlag(USARTx,USART_FLAG_TC);					//发送字符后清空标志位
		i++;
	}
}

void chuankou_init(void){
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

	GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);//重映射
		
	GPIO_InitTypeDef mm;//配置发送脚
	mm.GPIO_Mode=GPIO_Mode_AF_PP;
	mm.GPIO_Pin=GPIO_Pin_6;
	mm.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&mm);
	
	USART_InitTypeDef ppl;
	ppl.USART_BaudRate=9600;
	ppl.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	ppl.USART_Mode=USART_Mode_Tx;
	ppl.USART_Parity=USART_Parity_No;
	ppl.USART_StopBits=USART_StopBits_1;
	ppl.USART_WordLength=USART_WordLength_8b;
	USART_Init(USART1,&ppl);	
	USART_Cmd(USART1,ENABLE);//使能
}

