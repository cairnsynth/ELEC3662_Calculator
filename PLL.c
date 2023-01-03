#include "PLL.h"

void pll_init(void) {																					//http://users.ece.utexas.edu/~valvano/Volume1/E-Book/C10_FiniteStateMachines.htm
	SYSCTL_RCC2_R |= 0x80000000;
	SYSCTL_RCC2_R |= 0x00000800;
	SYSCTL_RCC_R = (SYSCTL_RCC_R & ~0x000007C0) + 0x00000540;
	SYSCTL_RCC2_R &= ~0x00000070;
	SYSCTL_RCC2_R &= ~0x00002000;
	SYSCTL_RCC2_R |= 0x40000000;
	SYSCTL_RCC2_R = (SYSCTL_RCC2_R & ~0x1FC00000) + (4 << 22);
	while((SYSCTL_RIS_R & 0x00000040) == 0) {};
	SYSCTL_RCC2_R &= ~0x00000800;
		
	NVIC_ST_CTRL_R = 0;
	NVIC_ST_RELOAD_R = 0x00FFFFFF;
	NVIC_ST_CURRENT_R = 0;
	NVIC_ST_CTRL_R = 0x00000005;
}

void pll_delay_100u() {
	NVIC_ST_RELOAD_R = 0x00001F3F;																//100us = 8000*12.5ns,		time = delay - 1 = 7999
	NVIC_ST_CURRENT_R = 0;
	while ((NVIC_ST_CTRL_R & 0x00010000) == 0) {}
}

void pll_delay_us(unsigned long count) {
	unsigned int c;
	NVIC_ST_RELOAD_R = 0x0000004F;																//1us = 80*12.5ns,			time = delay - 1 = 79
	NVIC_ST_CURRENT_R = 0;																				//Reset counter register
	for(c = 0; c < count; c++) {
		while((NVIC_ST_CTRL_R & 0x00010000) == 0) {}									//Wait until overflow bit is high
	}
}

void pll_delay_ms(unsigned long count) {
	unsigned int c;
	unsigned int m;
	for (c = 0; c < count; c++) {
		for(m = 0; m < 10; m++) {
			pll_delay_100u();
		}
	}
}