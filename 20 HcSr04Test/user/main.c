#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"

void App_USART1_Init(void);
void App_HCSR04_Init(void);

int main(void)
{
  Delay_Init();

  App_USART1_Init();
  App_HCSR04_Init();
  My_USART_SendString(USART1, "鼠鼠我鸭");

  while(1)
  {
    //向CNT写0
    TIM_SetCounter(TIM1, 0);

    //清除CC1和CC2标志位
    TIM_ClearFlag(TIM1, TIM_FLAG_CC1);
    TIM_ClearFlag(TIM1, TIM_FLAG_CC2);

    //开启定时器
    TIM_Cmd(TIM1, ENABLE);

    //向TRGI发送10us的脉冲
    GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);
    //延时10
    Delay(10);
    //停止脉冲
    GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);

    //等待测量完成
    while(TIM_GetFlagStatus(TIM1, TIM_FLAG_CC1) == RESET);
    while(TIM_GetFlagStatus(TIM1, TIM_FLAG_CC2) == RESET);

    //读取CCR1 CCR2
    uint16_t ccr1 = TIM_GetCapture1(TIM1);
    uint16_t ccr2 = TIM_GetCapture2(TIM1);

    //关闭定时器
    TIM_Cmd(TIM1, DISABLE);


    float distance = (ccr2 - ccr1) * 1.0e-6f * 340.0f / 2;

    My_USART_Printf(USART1, "distance is %.3f\n", distance * 100);


    //延时100
    Delay(100);

  }
}

void App_USART1_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  //PA9 TX AFPP
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;

  GPIO_Init(GPIOA, & GPIO_InitStruct);

  //PA10  RX IPU
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;

  GPIO_Init(GPIOA, & GPIO_InitStruct);


  //初始化usart1
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

  USART_InitTypeDef USART_InitStruct;

  USART_InitStruct.USART_BaudRate = 115200;
  USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
  USART_InitStruct.USART_Parity = USART_Parity_No;
  USART_InitStruct.USART_StopBits = USART_StopBits_1;
  USART_InitStruct.USART_WordLength = USART_WordLength_8b;

  USART_Init(USART1, & USART_InitStruct);

  //usart1总开关
  USART_Cmd(USART1, ENABLE);

}

void App_HCSR04_Init(void)
{
  //初始化时基单元
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct = {0};

  TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInitStruct.TIM_Period = 65535;
  TIM_TimeBaseInitStruct.TIM_Prescaler = 71;
  TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM1, & TIM_TimeBaseInitStruct);

  //初始化输入捕获io引脚
  //PA8 IPD
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_InitTypeDef GPIO_InitStruct;

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;

  GPIO_Init(GPIOA, & GPIO_InitStruct);

  //初始化输入捕获通道1

  TIM_ICInitTypeDef TIM_ICInitStruct = {0};

  TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
  TIM_ICInitStruct.TIM_ICFilter = 0;
  TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;

  TIM_ICInit(TIM1, & TIM_ICInitStruct);


  //初始化输入捕获通道2

  TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;
  TIM_ICInitStruct.TIM_ICFilter = 0;
  TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Falling;
  TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_IndirectTI;

  TIM_ICInit(TIM1, & TIM_ICInitStruct);

  //初始化Trig引脚
  // PA0 OUTPP
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOA, & GPIO_InitStruct);
}

