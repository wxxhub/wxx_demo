#include "sys_tick.h"

static __IO u32 TimingDelay;

void initSysTick(void) {
    // SystemFrequency / 1000     1ms中断一次
	 // SystemFrequency / 100000	 10us中断一次
	 // SystemFrequency / 1000000  1us中断一次
    if (SysTick_Config(SystemCoreClock/1000000)) {
        /* Capture error */ 
        while (1);
    }
}

void delay_us(__IO u32 n_time) {
    TimingDelay = n_time;
    while (TimingDelay != 0);
}

// 需要在中断函数SysTick_Handler()中调用
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
	TimingDelay--;
	}
}
