#include "stm32f10x.h"
#include "delay.h"
#include "max7219.h"


int main(void)
{
  Delay_Init();

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

  GPIO_InitTypeDef GPIO_InitStruct;

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOC, & GPIO_InitStruct);



  //初始化引脚
  //A0 DIN
  //A1 CS
  //A2 CLK
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

//	GPIO_InitTypeDef GPIO_InitStruct;

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_Init(GPIOA, & GPIO_InitStruct);


  Max7219_Init();


  uint8_t broken_heart_left_map[] = {
    /*--  调入了一幅图像：这是您新建的图像  --*/
    /*--  宽度x高度=8x8  --*/
    0x1C, 0x3E, 0x7E, 0xB4, 0x00, 0x00, 0x00, 0x00,
  };
  uint8_t broken_heart_right_map[] = {
    /*--  调入了一幅图像：这是您新建的图像  --*/
    /*--  宽度x高度=8x8  --*/
    0x00, 0x00, 0x00, 0x48, 0xFC, 0x7E, 0x3E, 0x1C,
  };

  uint8_t  TableL[] = {
    0x10, 0x20, 0x7F, 0xC2, 0x5A, 0x5A, 0x42, 0x46,
  };


  uint8_t  number1[] = {
   	0x08,0x38,0x18,0x18,0x18,0x18,0x18,0x3C,
  };

 

  uint8_t  heart1[] = {
    0x00, 0x66, 0xFF, 0xFF, 0xFF, 0x7E, 0x3C, 0x18,
  };

  Max7219_Clear();

  Max7219_SetBrightness(L0);
  //Max7219_DrawBitMap(broken_heart_left_map, 8, 8, 0, 0);
  //Max7219_DrawBitMap(broken_heart_right_map, 8, 8, 0, 0);


  while(1)
  {
    GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
    Delay(50);
    GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
    Delay(1000);

    for(int i = -7; i < 1; i++)
    {
      Max7219_Clear();
      Max7219_DrawBitMap(broken_heart_left_map, 8, 8, i, 0);
      Max7219_DrawBitMap(broken_heart_right_map, 8, 8, -i, 0);
      Delay(300);
    }

    Delay(1000);

    Max7219_Clear();
    Delay(1000);
    Max7219_DrawBitMap(TableL, 8, 8, 0, 0);
    Delay(1000);
 
    Max7219_Clear();
    Delay(1000);
    Max7219_DrawBitMap(heart1, 8, 8, 0, 0);
    Delay(1000);

		
		
    Max7219_Clear();
    Delay(1000);
    Max7219_DrawBitMap(number1, 8, 8, 0, 0);
    Delay(1000);
		
    Max7219_Fill();
    Delay(1000);

    Max7219_Clear();
  }


}



