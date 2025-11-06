#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"

void My_USART_Init(void);
void My_OnBoardLED_Init(void);

int main(void)
{
  Delay_Init();
  My_USART_Init();
  My_OnBoardLED_Init();

//  uint8_t bytes[] = {0, 1, 2, 3, 4, 5};

  while(1)
  {
//		My_USART_SendBytes(USART1,bytes, 6);
//		My_USART_SendByte(USART1,0x5a);
//			My_USART_SendChar(USART1,'a');
//		My_USART_SendString(USART1," Hello World \r\n");
//		My_USART_Printf(USART1," Hello World2 \r\n");
//    char ch = My_USART_ReceiveByte(USART1);
    char buffer[256];

    int len = 	My_USART_ReceiveLine(USART1, buffer, 256, LINE_SEPERATOR_LF, 10);

    if(len == 0) {
      My_USART_SendString(USART1, buffer);
      GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
//      Delay(1000);
    } else {
      GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
//      Delay(1000);
    }

//    My_USART_SendChar(USART1, ch);
  }
}



void My_USART_Init(void)
{
  //引脚初始化
  GPIO_InitTypeDef GPIO_InitStruct;

  // PA9 TX 复用输出推挽
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(GPIOA, & GPIO_InitStruct);

  // PA10 RX 复用输出开漏
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOA, & GPIO_InitStruct);


//	//重映射USART引脚到PB6 PB7

//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
//	GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);


//	//PB6   TX 重映射 输出推挽
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);


//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStruct);

//		//PB7  RX 重映射 输出开漏
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
//	GPIO_Init(GPIOB, &GPIO_InitStruct);



  //USART初始化
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

  USART_InitTypeDef USART_InitStruct;
  USART_InitStruct.USART_BaudRate = 115200;
  USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
  USART_InitStruct.USART_WordLength = USART_WordLength_8b;
  USART_InitStruct.USART_StopBits = USART_StopBits_1;
  USART_InitStruct.USART_Parity = USART_Parity_No;

  USART_Init(USART1, & USART_InitStruct);

  USART_Cmd(USART1, ENABLE); //打开USART1总开关

}



void My_OnBoardLED_Init(void) {

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

  //引脚初始化
  GPIO_InitTypeDef GPIO_InitStruct;

  // PC13 输出开漏

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOC, & GPIO_InitStruct);
}
