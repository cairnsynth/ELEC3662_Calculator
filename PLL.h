#ifndef PLL_H
#define PLL_H

#define SYSCTL_RCC_R					(*((volatile unsigned long *) 0x400FE060))
#define SYSCTL_RCC2_R					(*((volatile unsigned long *) 0x400FE070))
#define SYSCTL_RIS_R					(*((volatile unsigned long *) 0x400FE050))

#define NVIC_ST_CTRL_R 				(*((volatile unsigned long *) 0xE000E010))
#define NVIC_ST_RELOAD_R			(*((volatile unsigned long *) 0xE000E014))
#define NVIC_ST_CURRENT_R			(*((volatile unsigned long *) 0xE000E018))

void pll_init(void);												//Initialise PLL
void pll_delay_100u(void);									//100us delay
void pll_delay_us(unsigned long count);
void pll_delay_ms(unsigned long count);			//(count)ms delay

#endif //PLL_H