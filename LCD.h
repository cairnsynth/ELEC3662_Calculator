#ifndef LCD_H
#define LCD_H

//Clock control register
#define SYSCTL_RCG2_R (* ((volatile unsigned long *) 0x400FE108))

//Port A registers
#define GPIO_PORTA_DIR_R        (*((volatile unsigned long *) 0x40004400))
#define GPIO_PORTA_AFSEL_R      (*((volatile unsigned long *) 0x40004420))
#define GPIO_PORTA_PUR_R        (*((volatile unsigned long *) 0x40004510))
#define GPIO_PORTA_DEN_R        (*((volatile unsigned long *) 0x4000451C))
#define GPIO_PORTA_LOCK_R       (*((volatile unsigned long *) 0x40004520))
#define GPIO_PORTA_CR_R         (*((volatile unsigned long *) 0x40004524))
#define GPIO_PORTA_AMSEL_R      (*((volatile unsigned long *) 0x40004528))
#define GPIO_PORTA_PCTL_R       (*((volatile unsigned long *) 0x4000452C))
	
#define EN											(*((volatile unsigned long *) 0x40004010))
#define RS  										(*((volatile unsigned long *) 0x40004020))

//Port B registers
#define DATA				(*((volatile unsigned long *) 0x400053FC))

#define GPIO_PORTB_DIR_R        (*((volatile unsigned long *) 0x40005400))
#define GPIO_PORTB_AFSEL_R      (*((volatile unsigned long *) 0x40005420))
#define GPIO_PORTB_PUR_R        (*((volatile unsigned long *) 0x40005510))
#define GPIO_PORTB_DEN_R        (*((volatile unsigned long *) 0x4000551C))
#define GPIO_PORTB_LOCK_R       (*((volatile unsigned long *) 0x40005520))
#define GPIO_PORTB_CR_R         (*((volatile unsigned long *) 0x40005524))
#define GPIO_PORTB_AMSEL_R      (*((volatile unsigned long *) 0x40005528))
#define GPIO_PORTB_PCTL_R       (*((volatile unsigned long *) 0x4000552C))
	
#define DEBUG_ON 0

//Function definitions
void lcd_init(void);											//Initialise LCD
void lcd_en_pulse(void);											//Latch enable
void lcd_write_command(unsigned char c);	//Write command to LCD
void lcd_write_data(char c);							//Write data to LCD
void lcd_clear(void);											//Clear LCD
void lcd_print_char(char c);							//Print ASCII character to LCD
void lcd_print_char_literal(char c);
void lcd_print_string(char s[]);
void lcd_goto(unsigned char l, unsigned char p);					//Goto position on LCD
void debug_mess(char str[]);

enum char_type {
	SYMBOL,
	NUMBER,
	UPPERCASE,
	LOWERCASE
};

#endif //LCD_H