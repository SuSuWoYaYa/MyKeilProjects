#include "stm32f10x.h"
#include "delay.h"

void My_I2C_Init(void);

int My_I2C_SendBytes(I2C_TypeDef* I2Cx, uint8_t Addr, uint8_t* pData, uint16_t Size);

int main(void)
{
  Delay_Init();
//
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);

	uint8_t commads[] = {0x00, 0x8d, 0x14, 0xaf, 0xa5};

  My_I2C_Init();
	
	My_I2C_SendBytes(I2C1, 0x78, commads, 5);
	
  while(1)
  {
    GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_RESET);
    Delay(500);
    GPIO_WriteBit(GPIOC, GPIO_Pin_13, Bit_SET);
    Delay(1000);
  }
}


void My_I2C_Init(void) {
  // io引脚初始化
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  //重映射I2CI
  GPIO_PinRemapConfig(GPIO_Remap_I2C1, ENABLE);

  //PB8 PB9初始化 复用开漏
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;

  GPIO_Init(GPIOB, & GPIO_InitStruct);

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);  //开启i2c时钟
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, ENABLE);	//复位信号
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_I2C1, DISABLE); //释放复位信号

  I2C_InitTypeDef I2C_InitStruct;
  I2C_InitStruct.I2C_ClockSpeed = 400000;	//波特率400k
  I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;	// 标准的I2C
  I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;	//占空比 2:1
  I2C_Init(I2C1, &I2C_InitStruct);

  I2C_Cmd(I2C1, ENABLE); //闭合i2c1的总开关
}


int My_I2C_SendBytes(I2C_TypeDef* I2Cx, uint8_t Addr, uint8_t* pData, uint16_t Size) {
  //等待总线空闲
  while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_SB) == SET);

  //发送起始位
  I2C_GenerateSTART(I2Cx, ENABLE);
  while(I2C_GetFlagStatus(I2Cx, I2C_FLAG_SB) == RESET);

  //寻址阶段
  I2C_ClearFlag(I2Cx, I2C_FLAG_AF);
  I2C_SendData(I2Cx, Addr & 0xfe);

  while(1)
  {
    if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_ADDR) == SET) {
      break;
    }
    if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_AF) == SET) {

      I2C_GenerateSTOP(I2Cx, ENABLE);
      return -1;  //寻址失败
    }

  }


  //清除ADDR
  I2C_ReadRegister(I2Cx, I2C_Register_SR1);
  I2C_ReadRegister(I2Cx, I2C_Register_SR2);

  for(uint16_t i = 0; i < Size; i++) {
    while(1) {
      if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_AF) == SET) {
        I2C_GenerateSTOP(I2Cx, ENABLE);
      }
      if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_TXE) == SET) {
        break;
      }
    }
		//发送数据
    I2C_SendData(I2Cx, pData[i]);
  }


	while(1){
		if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_AF) == SET){
			I2C_GenerateSTOP(I2Cx, ENABLE);
			return -2; //数据被拒收
		}
		
		if(I2C_GetFlagStatus(I2Cx, I2C_FLAG_BTF) == SET){
			break;
		}
	}
	//数据发送成功 
	//发送停止位
	I2C_GenerateSTOP(I2Cx, ENABLE);
  return 0;
}

