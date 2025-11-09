#include "stm32f10x.h"
#include "delay.h"

int main(void)
{
	Delay_Init();
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
 
	
	Delay_Init();
	while(1)
	{
 
 
// 			循环点亮PC13 LED灯
			GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
			Delay(1000);
			GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
			Delay(1000);

	}
}
