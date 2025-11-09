#include "stm32f10x.h"
#include "delay.h"
#include "math.h"

void App_PWM_Init(void);

//中断时脉冲计数
float t = 0;

//呼吸间隔时间 单位毫秒
uint32_t delayTime = 2000;

int main(void)
{
  //修改课程代码加入中断, 中断里更新pwm
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

//  Delay_Init();
  App_PWM_Init();

  while(1)
  {

  }
}


void App_PWM_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  //PA8 AF_PP
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOA, & GPIO_InitStruct);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  //PB13 AF_PP
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOB, & GPIO_InitStruct);

  //开启定时器时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

  //初始化时基单元
  TIM_TimeBaseInitTypeDef TimeBaseInitStruct;
  TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;	// //TIM向上计数模式
  TimeBaseInitStruct.TIM_Period = delayTime - 1;  //重装载寄存器周期的值
  TimeBaseInitStruct.TIM_Prescaler = 71;	////设置用来作为TIMx时钟频率除数的预分频值
  TimeBaseInitStruct.TIM_RepetitionCounter = 0;
  TimeBaseInitStruct.TIM_ClockDivision = 0; //设置时钟分割

  TIM_TimeBaseInit(TIM1, & TimeBaseInitStruct);
  TIM_ClearFlag(TIM1, TIM_FLAG_Update);//清中断标志位

  //配置ARR寄存器的预加载
  TIM_ARRPreloadConfig(TIM1, ENABLE);


  //初始化输出比较
  //通道1
  TIM_OCInitTypeDef TIM_OCInitStruct;
  TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;  //模式1

  TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCNPolarity_High;//设置互补输出的极性
  TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;//设置正常输出的极性

  TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Enable;//开启开关
  TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; //开启开关
  TIM_OCInitStruct.TIM_Pulse = 0; //ccr捕获比较寄存器的初值 初始值设置为0
  TIM_OC1Init(TIM1, & TIM_OCInitStruct);

  //闭合MOE开关 高级定时器需要打开此开关
  TIM_CtrlPWMOutputs(TIM1, ENABLE);

  //配置CCR寄存器的预加载
  TIM_CCPreloadControl(TIM1, ENABLE);

  //配置TIM1_UP_IRQn中断
  TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);

  NVIC_InitTypeDef NVIC_InitStruct;

  NVIC_InitStruct.NVIC_IRQChannel = TIM1_UP_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init( & NVIC_InitStruct);

  //闭合总开关
  TIM_Cmd(TIM1, ENABLE);
}

void TIM1_UP_IRQHandler(void)
{
  //检查TIM1更新中断发生与否
  if(TIM_GetITStatus(TIM1, TIM_FLAG_Update) == SET)
  {
    //清除中断标志位
    TIM_ClearITPendingBit(TIM1, TIM_IT_Update);//清除TIMx的中断待处理位:TIM 中断源
    
    //t转换到毫秒单位 ， sin参数取值0~2PI，瞎写
    float floatT = t * 1.0e-3f;
		
		//产生0到1的正弦波时间
    float duty = 0.5 * (sinf(2 * 3.14 * floatT) + 1);
		
    //调整到对应比例的呼吸时间
    uint16_t ccr1 = duty* delayTime;
    //设置CCR1
    TIM_SetCompare1(TIM1, ccr1);

    //正弦波变量变化
    t++;

    //最大sin2Pi, sinf(2 * 3.14 * t)
    if(t > 1000)
    {
      t = 0;
    }
  }

}

