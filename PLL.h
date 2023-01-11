#ifndef PLL_H
#define PLL_H

/* register address definitions */
#define SYSCTL_RCC_R       (*((volatile unsigned long *) 0x400FE060))
#define SYSCTL_RCC2_R	   (*((volatile unsigned long *) 0x400FE070))
#define SYSCTL_RIS_R	   (*((volatile unsigned long *) 0x400FE050))
#define NVIC_ST_CTRL_R     (*((volatile unsigned long *) 0xE000E010))
#define NVIC_ST_RELOAD_R   (*((volatile unsigned long *) 0xE000E014))
#define NVIC_ST_CURRENT_R  (*((volatile unsigned long *) 0xE000E018))

/**
 * Initialise PLL
*/
void pll_init(void);

/**
 * 100 microsecond delay
*/
void pll_delay_100u(void);

/**
 * variable microsecond delay
 * 
 * count: delay(us)
*/
void pll_delay_us(unsigned long count);

/**
 * Variable millisecond delay
 * 
 * count: delay(ms)
*/
void pll_delay_ms(unsigned long count);

#endif  // PLL_H