#include "PLL.h"

/**
 * REF: Jonathan Valvano, Ramesh Yerraballi, Finite State Machines,
 * <//http://users.ece.utexas.edu/~valvano/Volume1/E-Book/C10_FiniteStateMachines.htm>
 * [accessed 12/01/2023]
*/
void pll_init(void) {
	SYSCTL_RCC2_R |= 0x80000000;
	SYSCTL_RCC2_R |= 0x00000800;  // set bypass
	// specify crystal frequency and select main oscillator
	SYSCTL_RCC_R = (SYSCTL_RCC_R & ~0x000007C0) + 0x00000540;
	SYSCTL_RCC2_R &= ~0x00000070;  
	SYSCTL_RCC2_R &= ~0x00002000;  // activate PLL
	SYSCTL_RCC2_R |= 0x40000000;  // configure clock divider
	SYSCTL_RCC2_R = (SYSCTL_RCC2_R & ~0x1FC00000) + (4 << 22);
	while((SYSCTL_RIS_R & 0x00000040) == 0) {}; // wait for PLL to stabilize
	SYSCTL_RCC2_R &= ~0x00000800;  // connect PLL
		
	NVIC_ST_CTRL_R = 0;  // disable in setup
	NVIC_ST_RELOAD_R = 0x00FFFFFF;  // max reload value
	NVIC_ST_CURRENT_R = 0;  // any write clears current
	NVIC_ST_CTRL_R = 0x00000005;  // enable with core clock
}

void pll_delay_100u() {
	//100us = 8000*12.5ns,		RELOAD_R = delay - 1 = 7999
	NVIC_ST_RELOAD_R = 0x00001F3F;
	//Reset counter register
	NVIC_ST_CURRENT_R = 0;
	//Poll overflow bit to delay
	while ((NVIC_ST_CTRL_R & 0x00010000) == 0) {}
}

void pll_delay_us(unsigned long count) {
	unsigned int c;
	for(c = 0; c < count; c++) {
		//1us = 80*12.5ns,      RELOAD_R = delay - 1 = 79 
		NVIC_ST_RELOAD_R = 0x0000004F;
		NVIC_ST_CURRENT_R = 0;
		while((NVIC_ST_CTRL_R & 0x00010000) == 0) {}
	}
}

void pll_delay_ms(unsigned long count) {
	unsigned int c;
	unsigned int m;
	// wait for (count*10) * 100us
	for (c = 0; c < count; c++) {
		for(m = 0; m < 10; m++) {
			pll_delay_100u();
		}
	}
}