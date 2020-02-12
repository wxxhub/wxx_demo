#ifndef _SYS_TICK_H_
#define _SYS_TICK_H_

#include "stm32f10x.h"

void initSysTick(void);
void TimingDelay_Decrement(void);
void delay_us(__IO u32 n_time);

#define delay_ms(x) delay_us(1000*x)

#endif /* _SYS_TICK_H_ */
