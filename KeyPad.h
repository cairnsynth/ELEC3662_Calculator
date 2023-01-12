#ifndef KEYPAD_H
#define KEYPAD_H

/**
 * KeyPad.h
 * ------------------
 * AUTHOR: Luke Coles
 * 
 * ------------------
 * Driver for 4x4 keypad peripheral, allowing scanning and decoding of
 * the button matrix
 * ------------------
 * REFS:
 * 
 * Microcontrollers Lab, 4×4 Keypad Interfacing with TM4C123 Tiva Launchpad,
 * <https://microcontrollerslab.com/4x4-keypad-interfacing-tm4c123-tiva-launchpad-keil/>
 * [accessed 12/01/2023]
 * 
*/

/* Register address definitions */
#define SYSCTL_RCG2_R       (*((volatile unsigned long *) 0x400FE108))
// Port D
#define GPIO_PORTD_DIR_R    (*((volatile unsigned long *) 0x40007400))
#define GPIO_PORTD_AFSEL_R  (*((volatile unsigned long *) 0x40007420))
#define GPIO_PORTD_PUR_R    (*((volatile unsigned long *) 0x40007510))
#define GPIO_PORTD_PDR_R    (*((volatile unsigned long *) 0x40007514))
#define GPIO_PORTD_DEN_R    (*((volatile unsigned long *) 0x4000751C))
#define GPIO_PORTD_LOCK_R   (*((volatile unsigned long *) 0x40007520))
#define GPIO_PORTD_CR_R     (*((volatile unsigned long *) 0x40007524))
#define GPIO_PORTD_AMSEL_R  (*((volatile unsigned long *) 0x40007528))
#define GPIO_PORTD_PCTL_R   (*((volatile unsigned long *) 0x4000752C))
#define GPIO_PORTD_DATA_R   (*((volatile unsigned long *) 0x400073FC))	
// Port E
#define GPIO_PORTE_DIR_R    (*((volatile unsigned long *) 0x40024400))
#define GPIO_PORTE_AFSEL_R  (*((volatile unsigned long *) 0x40024420))
#define GPIO_PORTE_PUR_R    (*((volatile unsigned long *) 0x40024510))
#define GPIO_PORTE_DEN_R    (*((volatile unsigned long *) 0x4002451C))
#define GPIO_PORTE_LOCK_R   (*((volatile unsigned long *) 0x40024520))
#define GPIO_PORTE_CR_R     (*((volatile unsigned long *) 0x40024524))
#define GPIO_PORTE_AMSEL_R  (*((volatile unsigned long *) 0x40024528))
#define GPIO_PORTE_PCTL_R   (*((volatile unsigned long *) 0x4002452C))	
#define GPIO_PORTE_DATA_R   (*((volatile unsigned long *) 0x400243FC))

// Enum avoids use of char literals, prevents bugs
enum Input {
	TIMES = 'x',
	DIVIDE = 7,
	PLUS = '+',
	MINUS = '-',
	DECIMAL = '.',
	EXPONENT = 'E',
	EQUALS = '=',
	PASSWORD = 'P',
	SHIFT = 'S',
	CLEAR = 'C',
	ALLCLEAR = 'A'
};

/**
 * Initialise keypad peripheral
*/
void keypad_init(void);

/**
 * Return key input from keypad
*/
unsigned char keypad_read(void);

#endif  // KEYPAD_H