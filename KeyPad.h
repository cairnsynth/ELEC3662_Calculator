#ifndef KEYPAD_H
#define KEYPAD_H

#define SYSCTL_RCG2_R (* ((volatile unsigned long *) 0x400FE108))

/*Port D Registers*/
#define GPIO_PORTD_DIR_R        (*((volatile unsigned long *) 0x40007400))
#define GPIO_PORTD_AFSEL_R      (*((volatile unsigned long *) 0x40007420))
#define GPIO_PORTD_PUR_R        (*((volatile unsigned long *) 0x40007510))
#define GPIO_PORTD_PDR_R				(*((volatile unsigned long *) 0x40007514))
#define GPIO_PORTD_DEN_R        (*((volatile unsigned long *) 0x4000751C))
#define GPIO_PORTD_LOCK_R       (*((volatile unsigned long *) 0x40007520))
#define GPIO_PORTD_CR_R         (*((volatile unsigned long *) 0x40007524))
#define GPIO_PORTD_AMSEL_R      (*((volatile unsigned long *) 0x40007528))
#define GPIO_PORTD_PCTL_R       (*((volatile unsigned long *) 0x4000752C))
	
#define GPIO_PORTD_DATA_R				(*((volatile unsigned long *) 0x400073FC))
	
/*Port E Registers*/
#define GPIO_PORTE_DIR_R        (*((volatile unsigned long *) 0x40024400))
#define GPIO_PORTE_AFSEL_R      (*((volatile unsigned long *) 0x40024420))
#define GPIO_PORTE_PUR_R        (*((volatile unsigned long *) 0x40024510))
#define GPIO_PORTE_DEN_R        (*((volatile unsigned long *) 0x4002451C))
#define GPIO_PORTE_LOCK_R       (*((volatile unsigned long *) 0x40024520))
#define GPIO_PORTE_CR_R         (*((volatile unsigned long *) 0x40024524))
#define GPIO_PORTE_AMSEL_R      (*((volatile unsigned long *) 0x40024528))
#define GPIO_PORTE_PCTL_R       (*((volatile unsigned long *) 0x4002452C))
	
#define GPIO_PORTE_DATA_R				(*((volatile unsigned long *) 0x400243FC))

/*Bit Specific Addresses*/
#define C1											(*((volatile unsigned long *) 0x40007004))
#define C2											(*((volatile unsigned long *) 0x40007008))
#define C3											(*((volatile unsigned long *) 0x40007010))
#define C4											(*((volatile unsigned long *) 0x40007020))
#define R1											(*((volatile unsigned long *) 0x40024004))
#define R2											(*((volatile unsigned long *) 0x40024008))
#define R3											(*((volatile unsigned long *) 0x40024010))
#define R4											(*((volatile unsigned long *) 0x40024020))
	
enum Input {
	TIMES = 'x',
	DIVIDE = 7,
	PLUS = '+',
	MINUS = '-',
	DECIMAL = '.',
	EXPONENT = 'E',
	EQUALS = '=',
	SHIFT = 'S',
	CLEAR = 'C',
	ALLCLEAR = 'A'
		};

/*Initialises keypad GPIO*/
void keypad_init(void);

/*Returns which key is pressed on keypad*/
unsigned char keypad_read(void);

#endif /*KEYPAD_H*/