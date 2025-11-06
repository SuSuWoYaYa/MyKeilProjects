#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "button.h"

void App_Button_Init(void);
void App_BoradLED_Init(void);

void App_USART1_Init(void);

/* 回调函数 */
void  button_pressed_cb(void);            // 按钮按下的回调函数
void  button_released_cb(void);           // 按钮松开的回调函数
void  button_clicked_cb(uint8_t clicks);  // 按钮敲击的回调函数
void  button_long_pressed_cb(uint8_t ticks);      // 按钮长按的回调函数

Button_TypeDef button1;
uint32_t cnt = 0;

int main(void)
{
  Delay_Init();
  App_BoradLED_Init();
  App_Button_Init();
//   App_BoradLED_Init();

//  uint8_t currentStatus = Bit_RESET, preStatus = Bit_RESET;


  App_USART1_Init();
//  App_Button_Init();
//  My_USART_Printf(USART1, "%d", cnt);


  My_USART_SendString(USART1, "Hello World");

  while(1)
  {

    My_Button_Proc( & button1);
//    preStatus = currentStatus;
//    currentStatus = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);


//    if(currentStatus != preStatus)
//    {
//      if(currentStatus == Bit_SET) {

//    if(GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13) == RESET) {
//      GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET); //灭灯
//      Delay(1000);
//    } else
//    {
//      GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);//亮灯
//      Delay(1000);

//    }
//      }
//    }



//    GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
//    Delay(50);
//    GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
//    Delay(1000);
  }
}

void App_Button_Init(void)
{
  Button_InitTypeDef Button_InitStruct;

  Button_InitStruct.GPIOx = GPIOA;
  Button_InitStruct.GPIO_Pin = GPIO_Pin_0;
  Button_InitStruct.ClickInterval = 0;
  Button_InitStruct.LongPressTime = 0;
  Button_InitStruct.LongPressTickInterval = 0;
  Button_InitStruct.button_clicked_cb = button_clicked_cb;
  Button_InitStruct.button_pressed_cb = 0;
  Button_InitStruct.button_released_cb = 0;
  Button_InitStruct.button_long_pressed_cb = button_long_pressed_cb;

  My_Button_Init( & button1, & Button_InitStruct);


}
void App_BoradLED_Init(void)
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

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOA, & GPIO_InitStruct);


  //初始化usart1
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

  USART_InitTypeDef USART_InitStruct;
  USART_InitStruct.USART_BaudRate = 115200;
  USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
  USART_InitStruct.USART_WordLength = USART_WordLength_8b;
  USART_InitStruct.USART_Parity = USART_Parity_No;
  USART_InitStruct.USART_StopBits = USART_StopBits_1;
  USART_Init(USART1, & USART_InitStruct);

  USART_Cmd(USART1, ENABLE);
}

void  button_pressed_cb(void)             // 按钮按下的回调函数
{
  cnt++;
  My_USART_Printf(USART1, "%d", cnt);
}

//void  button_released_cb(void)            // 按钮松开的回调函数
//{
//  cnt++;
//  My_USART_Printf(USART1, "%d", cnt);
//}

void  button_clicked_cb(uint8_t clicks)   // 按钮敲击的回调函数
{
  if(clicks == 1) {

    cnt++;
    My_USART_Printf(USART1, "%d", cnt);
  } else  if(clicks == 2) {

    cnt = 0;
    My_USART_Printf(USART1, "%d", cnt);
  }
}

void  button_long_pressed_cb(uint8_t ticks)       // 按钮长按的回调函数
{
  cnt++;
  My_USART_Printf(USART1, "%d", cnt);
}
