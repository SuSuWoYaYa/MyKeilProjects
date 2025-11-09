#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"

void App_USART1_Init(void);
void App_TIM1_Init(void);
void App_ADC1_Init(void);

int main(void)
{
  Delay_Init();

  App_USART1_Init();
  App_TIM1_Init();
  App_ADC1_Init();

  My_USART_SendString(USART1, "鼠鼠我鸭\n");

  while(1)
  {
    //清除EOC标志位
    ADC_ClearFlag(ADC1,ADC_FLAG_JEOC);

    //通过软启动的方式发送脉冲
    //ADC_SoftwareStartConvCmd(ADC1, ENABLE);

    //等待常规序列转换完成
    while(ADC_GetFlagStatus(ADC1,ADC_FLAG_JEOC) == RESET);

    //读取转换结果
    uint16_t jdr = ADC_GetInjectedConversionValue(ADC1,ADC_InjectedChannel_1);

    //把结果转换成电压
    float voltage = jdr*(3.3f / 4095);
		
		My_USART_Printf(USART1,"%.3f\n",voltage);
		
  }
}


void App_USART1_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

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

  USART_InitTypeDef USART_InitStruct = {0};

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


void App_TIM1_Init(void)
{
  //使能TIM1时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

  //配置时基单元
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct = {0};
  TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInitStruct.TIM_Period = 999;
  TIM_TimeBaseInitStruct.TIM_Prescaler = 71;
  TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM1, & TIM_TimeBaseInitStruct);

  //TRGO:UPdate
  TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_Update);

  TIM_Cmd(TIM1, ENABLE);
}


void App_ADC1_Init(void)
{
  //初始化PA0,模拟模式
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOA, & GPIO_InitStruct);

  //配置ADC模块时钟

  //6分频 ADC不超过14Mhz
  RCC_ADCCLKConfig(RCC_PCLK2_Div6);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

  ADC_InitTypeDef ADC_InitStruct = {0};
  ADC_InitStruct.ADC_ContinuousConvMode = DISABLE; //关闭连续模式
  ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right; //结果右对齐
  ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //无外部触发, 采用软件启动
  ADC_InitStruct.ADC_Mode = ADC_Mode_Independent; //独立模式
  ADC_InitStruct.ADC_NbrOfChannel = 1;  //常规序列1个通道
  ADC_InitStruct.ADC_ScanConvMode = DISABLE;

  ADC_Init(ADC1, & ADC_InitStruct);

  //配置注入序列长度
  ADC_InjectedSequencerLengthConfig(ADC1, 1);
	
	//配置注入通道1参数
  ADC_InjectedChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_13Cycles5);

  //配置TIM1 TRGO触发
  ADC_ExternalTrigInjectedConvConfig(ADC1, ADC_ExternalTrigInjecConv_T1_TRGO);
  ADC_ExternalTrigInjectedConvCmd(ADC1, ENABLE);

  //闭合总开关
  ADC_Cmd(ADC1, ENABLE);
}


