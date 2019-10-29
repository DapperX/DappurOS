#include "timer.h"
#include "arch/x86/io.h"
#include "8253.h"

void timer_set(u32 freq_timer)
{
	u32 cnt = (PIT_FREQUENCY + freq_timer/2)/freq_timer;
	// cnt==1 is illegal in mode 2
	if(cnt==1) cnt = 2;
	io_out_b(PORT_PIT_CTRL, PIT_READ_LH|PIT_MODE_2), io_delay();
	io_out_b(PORT_PIT_CNT0, (u8)(cnt&0xff)), io_delay();
	io_out_b(PORT_PIT_CNT0, (u8)((cnt>>8)&0xff)), io_delay();
}