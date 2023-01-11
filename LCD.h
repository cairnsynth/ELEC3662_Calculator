#ifndef LCD_H
#define LCD_H

/* register address definitions*/
#define SYSCTL_RCG2_R (* ((volatile unsigned long *) 0x400FE108))
// port A
#define GPIO_PORTA_DIR_R    (*((volatile unsigned long *) 0x40004400))
#define GPIO_PORTA_AFSEL_R  (*((volatile unsigned long *) 0x40004420))
#define GPIO_PORTA_PUR_R    (*((volatile unsigned long *) 0x40004510))
#define GPIO_PORTA_DEN_R    (*((volatile unsigned long *) 0x4000451C))
#define GPIO_PORTA_LOCK_R   (*((volatile unsigned long *) 0x40004520))
#define GPIO_PORTA_CR_R     (*((volatile unsigned long *) 0x40004524))
#define GPIO_PORTA_AMSEL_R  (*((volatile unsigned long *) 0x40004528))
#define GPIO_PORTA_PCTL_R   (*((volatile unsigned long *) 0x4000452C))
// port B
#define DATA			    (*((volatile unsigned long *) 0x400053FC))
#define GPIO_PORTB_DIR_R    (*((volatile unsigned long *) 0x40005400))
#define GPIO_PORTB_AFSEL_R  (*((volatile unsigned long *) 0x40005420))
#define GPIO_PORTB_PUR_R    (*((volatile unsigned long *) 0x40005510))
#define GPIO_PORTB_DEN_R    (*((volatile unsigned long *) 0x4000551C))
#define GPIO_PORTB_LOCK_R   (*((volatile unsigned long *) 0x40005520))
#define GPIO_PORTB_CR_R     (*((volatile unsigned long *) 0x40005524))
#define GPIO_PORTB_AMSEL_R  (*((volatile unsigned long *) 0x40005528))
#define GPIO_PORTB_PCTL_R   (*((volatile unsigned long *) 0x4000552C))
// E/RS pins
#define EN					(*((volatile unsigned long *) 0x40004010))
#define RS  				(*((volatile unsigned long *) 0x40004020))

// enable/disable debug messages
#define DEBUG_ON 0

/**
 * Initialise LCD
 * initialises port A and port B, initialises lcd by instruction,
 * loads custom characters into CGRAM
*/
void lcd_init(void);

/**
 * Pulses the enable pin
 * required by the LCD to load data/commands
 * shown in FIG 17 of HD44780U datasheet
*/
void lcd_en_pulse(void);

/**
 * Write 8bit command to command register
 * commands are sent in 2x 4bit writes to LCD
 * b7-4  - then b3-0
 * 
 * c: command to be written
*/
void lcd_write_command(unsigned char c);

/**
 * Write 4bits of data to LCD over data pins
 * used in lcd_write_command, lcd_print_char, functions
 * 
 * c: data to send, b3-0
*/
void lcd_write_data(char c);

/**
 * Clear LCD, set cursor to (0, 0)
*/
void lcd_clear(void);

/**
 * Print ASCII char to LCD display
 * 
 * c: ASCII char to print
*/
void lcd_print_char(char c);

/**
 * Print a char as its binary value
 * 
 * c: char to print
*/
void lcd_print_char_literal(char c);

/**
 * Print a char array to the LCD
 * printing wraps to line 2 if started on line 1
 * 
 * s[]: char array to print
*/
void lcd_print_string(char s[]);

/**
 * Move cursor to new position on LCD
 * changes DDRAM address to move print location
 * 
 * l: line (0 = line 1, 1 = line 2)
 * p: position (0 - 16)
*/
void lcd_goto(unsigned char l, unsigned char p);

/**
 * Briefly display a debug message on the LCD
 * used in development to debug when serial was not available
 * 
 * s[]: message
*/
void debug_mess(char str[]);

/**
 * Load a custom character into CGRAM
 * characters can be defined by char arrays
 * 
 * character: 8 item char array
 * location: CGRAM location (0 - 7)
*/
void lcd_custom_character(unsigned char* character, char location);

/**
 * Displays splash animation on LCD
*/
void lcd_splash_animation(void);

#endif  // LCD_H