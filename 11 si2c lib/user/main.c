#include "stm32f10x.h"
#include "delay.h"
#include "si2c.h"

int main(void)
{
	Delay_Init();
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	SI2C_TypeDef si2c;
	si2c.SCL_GPIOx = GPIOA;
	si2c.SCL_GPIO_Pin = GPIO_Pin_0;
	si2c.SDA_GPIOx = GPIOA;
	si2c.SDA_GPIO_Pin = GPIO_Pin_1;
	
	My_SI2C_Init(&si2c);
	
	uint8_t commads[] = {0x00, 0x8d, 0x14, 0xaf, 0xa5};
	My_SI2C_SendBytes(&si2c, 0x78, commads, 5);
	
	
	uint8_t rcvd;
	My_SI2C_ReceiveBytes(&si2c,0x78, &rcvd ,1);
	if((rcvd & (0x01 << 6)) == 0){
		GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
		Delay(10);
	}
	
	while(1)
	{
		
		 
	}
}
