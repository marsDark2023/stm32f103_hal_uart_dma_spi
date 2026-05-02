#include "main.h"
#include "config.h"
#ifdef USEMYSYSTICK

unsigned int
timer_counter = 0;

int
sysTickConfig(unsigned int time){
    if((time - 1) > SysTick_LOAD_RELOAD_Msk){
        return 1;
    }
    SysTick->LOAD = (unsigned int ) time - 1;
    NVIC_SetPriority(SysTick_IRQn, ((1UL << __NVIC_PRIO_BITS) - 1));
    SysTick->VAL = 0UL;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk| SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
    return 0;
}
 int
sysTickInit(void){
    if(sysTickConfig(SystemCoreClock / 100000)){
        while(1);
    }
    return 0;
}

void
SysTick_Handler_unused(void){
    if(timer_counter > 0 ){
        timer_counter -= 1;
    }
}
 int
mydelay(unsigned int time){
    timer_counter = time ;
    while (timer_counter>0){};
    return 0;
}
#else
 int
mydelay(unsigned int time){
    HAL_Delay(time);
    return 0 ;
}
#endif //USEMYDELAY
