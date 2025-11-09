#include "stm32f10x.h"
#include "delay.h"


void My_USART_SendBytes(USART_TypeDef* USARTx, uint8_t* pData, uint16_t Size);
void My_USART_SendByte(USART_TypeDef* USARTx, uint8_t Data);
void My_OnBoardLED_Init(void);
void My_USART_Init(void);

int main(void)
{
  Delay_Init();

  My_USART_Init();
  My_OnBoardLED_Init();

//  USART_ClearFlag(USART1, USART_FLAG_RXNE);

  while(1)
  {
//    USART_ReceiveData(USART1);
    while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
    uint8_t byteRcvd = USART_ReceiveData(USART1);

    if(byteRcvd == '0') {
      GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);		//灭灯
    }
    else if(byteRcvd == '2') {
      GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);		//灭灯
    }
    else if(byteRcvd == '1') {
      GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);  //亮灯
    }
    My_USART_SendByte(USART1, byteRcvd);

//    uint8_t flag =  USART_GetFlagStatus(USART1, USART_FLAG_RXNE);
//    if(flag == RESET) {
//      My_USART_SendByte(USART1, '0');
//    }
//    else {
//      My_USART_SendByte(USART1, '1');
//    }

    // 	GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET); //亮灯
   Delay(1000);
//    GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET); //灭灯
//    Delay(1000);
//		GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET); //亮灯
  }
}


void My_USART_SendBytes(USART_TypeDef* USARTx, uint8_t* pData, uint16_t Size)
{
  for(uint32_t i = 0; i < Size; i++) {
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USARTx, pData[i]);

  }

  while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
}

void My_USART_SendByte(USART_TypeDef* USARTx, uint8_t Data)
{

  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
  USART_SendData(USARTx,  Data);

  while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
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

