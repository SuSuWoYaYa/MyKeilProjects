#include "stm32f10x.h"
#include "delay.h"
#include "math.h"

void App_PWM_Init(void);

int main(void)
{
Delay_Init();
  App_PWM_Init();

  while(1)
  {
    float t = GetTick() * 1.0e-3f;
//		float getsin = sin(2 * 3.14 * t);
//		float sinAdd1=getsin +1;
//		float halfSin = 0.5 * sinAdd1;
//		float duty=halfSin;
    float duty = 0.5 * (sin(2 * 3.14 * t) + 1);
    uint16_t ccr1 = duty * 10000;
    TIM_SetCompare1(TIM1, ccr1);
  }


}


//void App_PWM_Init(void)
//{
//  GPIO_InitTypeDef GPIO_InitStruct;
//
//
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//  //PA8 AF_PP
//  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
//  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
//  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
//  GPIO_Init(GPIOA, & GPIO_InitStruct);

//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//  //PB13 AF_PP
//  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
//  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
//  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
//  GPIO_Init(GPIOB, & GPIO_InitStruct);

//  //开启定时器时钟
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

//  //初始化时基单元
//  TIM_TimeBaseInitTypeDef TimeBaseInitStruct;
//  TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
//  TimeBaseInitStruct.TIM_Period = 999;
//  TimeBaseInitStruct.TIM_Prescaler = 71;
//  TimeBaseInitStruct.TIM_RepetitionCounter = 0;
//  TIM_TimeBaseInit(TIM1, & TimeBaseInitStruct);


//  //配置ARR寄存器的预加载
//  TIM_ARRPreloadConfig(TIM1, ENABLE);

//  //闭合总开关
//  TIM_Cmd(TIM1, ENABLE);

//  //初始化输出比较
//  //通道1
//  TIM_OCInitTypeDef TIM_OCInitStruct;
//  TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;  //模式1
//
//  TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCNPolarity_High;//设置互补输出的极性
//  TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;//设置正常输出的极性
//
//  TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Enable;//开启开关
//  TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //开启开关
//  TIM_OCInitStruct.TIM_Pulse = 0; //ccr捕获比较寄存器的初值 初始值设置为0
//  TIM_OC1Init(TIM1, & TIM_OCInitStruct);

//  //闭合MOE开关 高级定时器需要打开此开关
//  TIM_CtrlPWMOutputs(TIM1, ENABLE);

//  //配置CCR寄存器的预加载
//  TIM_CCPreloadControl(TIM1, ENABLE);
//}


void App_PWM_Init(void)
{
  //1. 配置GPIO口
  // 开启时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  GPIO_InitTypeDef GPIO_InitStruct;
  //配置 GPIOA - pin8
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOA, & GPIO_InitStruct);

  //配置GPIOB - pin13
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOB, & GPIO_InitStruct);

  //2. 配置时基单元参数
  //2.1 开启定时器1时钟信号
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

  //2.2 配置时基单元参数（TIM）
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;

  TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInitStruct.TIM_Prescaler = 71;
  TIM_TimeBaseInitStruct.TIM_Period = 9999;
  TIM_TimeBaseInit(TIM1, & TIM_TimeBaseInitStruct);

  //2.3 开启自动重载寄存器预加载
  TIM_ARRPreloadConfig(TIM1, ENABLE);
  //2.4 开启总开关
  TIM_Cmd(TIM1, ENABLE);

  //配置输出比较模块
  TIM_OCInitTypeDef TIM_OCInitStruct;
  TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
  //设置互补输出的极性
  TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCNPolarity_High;
  //设置正常输出的极性
  TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
  //使能后面的开关
  TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
  //捕获比较寄存器的初值 设置为0
  TIM_OCInitStruct.TIM_Pulse = 0;
  TIM_OC1Init(TIM1, & TIM_OCInitStruct);

  //3.闭合MOE总开关
  TIM_CtrlPWMOutputs(TIM1, ENABLE);
  // 开启比较寄存器预加载
  TIM_CCPreloadControl(TIM1, ENABLE);
}

