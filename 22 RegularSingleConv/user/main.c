#include "stm32f10x.h"
#include "delay.h"

void App_ADC1_Init(void);
void App_OnBoardOLED_Init(void);

int main(void)
{
  Delay_Init();

  App_ADC1_Init();
  App_OnBoardOLED_Init();

  while(1)
  {
    //清除EOC标志位
    ADC_ClearFlag(ADC1, ADC_FLAG_EOC);

    //通过软启动的方式发送脉冲
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

    //等待常规序列转换完成
    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);

    //读取转换结果
    uint16_t dr = ADC_GetConversionValue(ADC1);

    //把结果转换成电压
    float voltage = dr*(3.3f / 4095);

//	if(voltage > 2.2)
    if(voltage > 1.5)
    {
      GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);

    }
    else
    {
      GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
    }
  }
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

  //配置常规序列
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_13Cycles5);

  ADC_ExternalTrigConvCmd(ADC1, ENABLE);

  //闭合总开关
  ADC_Cmd(ADC1, ENABLE);
}

void App_OnBoardOLED_Init(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOC, & GPIO_InitStruct);
}
