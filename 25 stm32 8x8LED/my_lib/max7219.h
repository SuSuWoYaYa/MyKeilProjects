//
// Created by DELL on 2022/9/10.
// 感谢https://github.com/Temperature6/Stm32Max7219GraphicsLibrary
//

#ifndef LATTICE_TEST_MAX7219_H
#define LATTICE_TEST_MAX7219_H

#include "stm32f10x.h"

/*
 DIN A0
 CS A1
 CLK A2
*/
#define CS_On GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_SET)
#define CS_Off GPIO_WriteBit(GPIOA, GPIO_Pin_1, Bit_RESET)
#define DIN_On GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET)
#define DIN_Off GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET)
#define CLK_On GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_SET)
#define CLK_Off GPIO_WriteBit(GPIOA, GPIO_Pin_2, Bit_RESET)

#define REG_DECODE        0x09
#define REG_INTENSITY     0x0A
#define REG_LIMIT         0x0B
#define REG_SHUTDOWN      0x0C
#define REG_TEST          0x0F

enum Brightness
{
    L0 = 0x00,
    L1,
    L2,
    L3,
    L4,
    L5,
    L6,
    L7,
    L8,
    L9,
    L10,
    L11,
    L12,
    L13,
    L14,
    L15,
};

enum State
{
    ON = 1,
    OFF = 0
};


//初始化点阵屏
//！！必须在GPIO初始化完成之后调用
void Max7219_Init(void);

//清空屏幕
void Max7219_Clear(void);

//单独控制一个LED
//x：横坐标
//y：纵坐标
//state：状态，ON（开）或 OFF（关）
void Max7219_SetDot(uint8_t x, uint8_t y, uint8_t state);

//设置亮度
void Max7219_SetBrightness(uint8_t brightness);

//填充整个屏幕
void Max7219_Fill(void);

//绘图
//map[]：位图数组，可由字模提取软件生成
//length：位图的长（像素
//height：位图的宽（像素）
//start_X：开始绘图的起始横坐标
//start_Y：开始绘图的起始纵坐标
//特性：绘图程序只绘制位图数组中有点亮的部分，未点亮的部分显示原来的状态
void Max7219_DrawBitMap(uint8_t map[], uint8_t length, uint8_t height, int start_X, int start_Y);

//反转屏幕（反显）
void Max7219_InvertScreen(void);

//开启或关闭屏幕
void Max7219_Power(uint8_t shutdown);

void max7219_write_byte(uint8_t data);
void max7219_write(uint8_t addr, uint8_t data);

#endif //LATTICE_TEST_MAX7219_H
