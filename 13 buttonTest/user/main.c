#include "stm32f10x.h"
#include "delay.h"

void App_Button_Init(void);
void App_BoradLED_Init(void);

int main(void)
{
  Delay_Init();

  App_Button_Init();
  App_BoradLED_Init();

  uint8_t currentStatus = Bit_RESET, preStatus = Bit_RESET;


  while(1)
  {

    preStatus = currentStatus;
    currentStatus = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);


    if(currentStatus != preStatus)
    {
      if(currentStatus == Bit_SET) {

        if(GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13) == RESET) {
          GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET); //灭灯
          Delay(10);
        } else
        {
          GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);//亮灯
          Delay(10);

        }
      }
    }



//    GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
//    Delay(50);
//    GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
//    Delay(1000);
  }
}

void App_Button_Init(void)
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  GPIO_InitTypeDef GPIO_InitStruct;

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOA, & GPIO_InitStruct);

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
