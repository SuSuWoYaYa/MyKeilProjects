#include "stm32f10x.h"
#include "delay.h"

void App_BoradLED_Init(void);
void App_Button_Init(void);


int main(void)
{
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  App_BoradLED_Init();
  App_Button_Init();

  while(1)
  {

  }
}

void EXTI9_5_IRQHandler(void)
{
  if(EXTI_GetFlagStatus(EXTI_Line5) == SET)
  { //线5触发中断
    EXTI_ClearFlag(EXTI_Line5);			//清除中断标志位
    GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET); //亮灯

  }

  if(EXTI_GetFlagStatus(EXTI_Line6) == SET)
  { //线6触发中断
    EXTI_ClearFlag(EXTI_Line6);//清除中断标志位
    GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);//亮灯

  }
}


void App_BoradLED_Init(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

  GPIO_InitTypeDef GPIO_InitStruct;

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOC, & GPIO_InitStruct);
  GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
}

void App_Button_Init(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  GPIO_InitTypeDef GPIO_InitStruct;

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOA, & GPIO_InitStruct);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOA, & GPIO_InitStruct);


  //为EXTI5和EXTI6分配引脚
  //开启AFIO的时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  //线5选择端口A
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource5);
  //线6选择端口A
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource6);


  //Exti模块无需使能时钟
  //初始化EXTI的线
  EXTI_InitTypeDef EXTI_InitStruct;

  //配置EXTI_LINE5
  EXTI_InitStruct.EXTI_Line = EXTI_Line5;
  EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStruct.EXTI_LineCmd = ENABLE;
  EXTI_Init( & EXTI_InitStruct);
  //配置EXTI_LINE6
  EXTI_InitStruct.EXTI_Line = EXTI_Line6;
  EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStruct.EXTI_LineCmd = ENABLE;
  EXTI_Init( & EXTI_InitStruct);

  //配置中断
  NVIC_InitTypeDef NVIC_InitStruct;

  NVIC_InitStruct.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;

  NVIC_Init( & NVIC_InitStruct);
}

