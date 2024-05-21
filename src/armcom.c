#include "armcom.h"
#include "main.h"

#define CONFIG_CLOCK_FREQ   180000000 // stm32f429-180MHz

// Return the number of clock ticks for a given number of microseconds
uint32_t timer_from_us(uint32_t us)
{
    return us * (CONFIG_CLOCK_FREQ / 1000000);
}

// Return true if time1 is before time2.  Always use this function to
// compare times as regular C comparisons can fail if the counter
// rolls over.
uint8_t timer_is_before(uint32_t time1, uint32_t time2)
{
    return (int32_t)(time1 - time2) < 0;
}

// Return the current time (in absolute clock ticks).
uint32_t
timer_read_time(void)
{
    return DWT->CYCCNT;
}

// Implement simple early-boot delay mechanism
void
udelay(uint32_t usecs)
{
    if (!(CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk)) {
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; 			
        DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;    							
    }
																																
    uint32_t end = timer_read_time() + timer_from_us(usecs);
    while (timer_is_before(timer_read_time(), end))
        ;
}

void armcom_timer_init(void) {

    // Enable Debug Watchpoint and Trace (DWT) for its 32bit timer
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;		// 使能DWT外设
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;                // 使能DWT
    DWT->CYCCNT = 0;                                    // 清空计数器
}


