#include "led.h"

void initLED(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    //打开PB口时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    //打开PE口时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

    //打开PB5, PE5口时钟
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;

    //端口速度
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    //端口模式， 此为输出推免模式
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

    //初始化对应端口
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_Init(GPIOE, &GPIO_InitStructure);
}
