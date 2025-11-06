#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "button.h"

void App_Button_Init(void);
void App_BoardLED_Init(void);
void App_USART1_Init(void);

/* 回调函数 */
void  button_clicked_cb(uint8_t clicks);  // 按钮敲击的回调函数
//void USART1_IRQHandler(void);

Button_TypeDef button1;
uint32_t blinkInerval = 1000;


int main(void)
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);


  Delay_Init();



//  App_BoardLED_Init();
  App_Button_Init();
  App_BoardLED_Init(); //usart不写流控的话, led在button后初始化导致usart不正常
  App_USART1_Init();

//  My_USART_Printf(USART1, "%d", cnt);


  My_USART_SendString(USART1, "Hello World");

  while(1)
  {
    My_Button_Proc( & button1);



    GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET); //亮
    Delay(blinkInerval);
    GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET); //灭
    Delay(blinkInerval);
  }
}


void USART1_IRQHandler(void)
{
  if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET)
  {
    uint8_t dataRcvd = USART_ReceiveData(USART1);

    if(dataRcvd == '0')
    {
      blinkInerval = 9999;
    } else if(dataRcvd == '1')
    {

      blinkInerval = 50;
    } else if(dataRcvd == '2')
    {

      blinkInerval = 200;
    } else if(dataRcvd == '3')
    {

      blinkInerval = 1000;
    } else
    {
      blinkInerval = 1000;
    }
  }

}


void App_Button_Init(void)
{
  Button_InitTypeDef Button_InitStruct = {0};

  Button_InitStruct.GPIOx = GPIOA;
  Button_InitStruct.GPIO_Pin = GPIO_Pin_0;
  Button_InitStruct.button_clicked_cb = button_clicked_cb;
  My_Button_Init( & button1, & Button_InitStruct);


}


void App_BoardLED_Init(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

  GPIO_InitTypeDef GPIO_InitStruct;

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOC, & GPIO_InitStruct);
}

void App_USART1_Init(void)
{
  // 初始化io引脚 PA9 TX , PA10 RX
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  GPIO_InitTypeDef GPIO_InitStruct;

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOA, & GPIO_InitStruct);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOA, & GPIO_InitStruct);


  //初始化usart1
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

  USART_InitTypeDef USART_InitStruct;
  USART_InitStruct.USART_BaudRate = 115200;
  USART_InitStruct.USART_HardwareFlowControl = SDIO_HardwareFlowControl_Disable;
  USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
  USART_InitStruct.USART_WordLength = USART_WordLength_8b;
  USART_InitStruct.USART_Parity = USART_Parity_No;
  USART_InitStruct.USART_StopBits = USART_StopBits_1;
  USART_Init(USART1, & USART_InitStruct);

  USART_Cmd(USART1, ENABLE);


  //配置中断
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

  //配置NVIC
  NVIC_InitTypeDef NVIC_InitStruct;
	
  NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	
  NVIC_Init( & NVIC_InitStruct);
}


void  button_clicked_cb(uint8_t clicks)   // 按钮敲击的回调函数
{
  if(clicks == 1) {


  }
}

