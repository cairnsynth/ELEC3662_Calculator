#include "LCD.h"
#include "PLL.h"
// pixel data for custom characters
unsigned char customDivide[8] = {0x00,0x04,0x00,0x1f,0x00,0x04,0x00,0x00};
unsigned char dith1[8]        = {0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f};
unsigned char dith2[8]        = {0x0a,0x17,0x0a,0x1d,0x0a,0x17,0x0a,0x1d};
unsigned char dith3[8]        = {0x15,0x08,0x15,0x02,0x15,0x08,0x15,0x0a};
unsigned char lock[8]         = {0x04,0x0A,0x0A,0x1F,0x1B,0x1B,0x1F,0x00};

void lcd_init(void) {
	volatile unsigned long delay;
	SYSCTL_RCG2_R |= 0x03;  // clock enable for Port A and B
	delay = SYSCTL_RCG2_R;  // wait for clock
	// port initialisation
	GPIO_PORTA_AMSEL_R &= 0x00;  // disable analog function
	GPIO_PORTA_PCTL_R  &= 0x00000000;  // PCTL GPIO on Port A
	GPIO_PORTA_DIR_R   |= 0xFF;  // set PA3-2 to output
	GPIO_PORTA_AFSEL_R &= 0x00;  // disable Port A alt function
	GPIO_PORTA_PUR_R   &= 0x00;  // disable pull-up resistors
	GPIO_PORTA_DEN_R   |= 0xFF;  // enable digital I/O on PA3-2
	
	GPIO_PORTB_AMSEL_R &= 0x00;  // disable analog function
	GPIO_PORTB_PCTL_R  &= 0x00000000;  // PCTL GPIO on Port B
	GPIO_PORTB_DIR_R   |= 0xFF;  // set PB3-0 to output
	GPIO_PORTB_AFSEL_R &= 0x00;  // disable Port B alt function
	GPIO_PORTB_PUR_R   &= 0x00;  // disable pull-up resistors
	GPIO_PORTB_DEN_R   |= 0xFF;  // enable digital I/O on PB3-0
	// LCD initialisation
	// non-specific delays ensure instruction has finished executing
	pll_delay_ms(100);         // wait >40ms after Vcc rises above 4.5V
	lcd_write_command(0x03);  // "Function set" instruction
	pll_delay_ms(5);          // wait >4.1ms (specified in datasheet)
	lcd_write_command(0x03);  // "Function set" x2
	pll_delay_us(100);        // wait >100us (specified in datasheet)
	lcd_write_command(0x03);  // "Function set" x3, triggers LCD reset
	pll_delay_us(100);
	lcd_write_command(0x02);  // set 4 bit interface
	pll_delay_us(100);
	lcd_write_command(0x02);  // set number of lines and font
	lcd_write_command(0x08);
	pll_delay_us(100);
	lcd_write_command(0x00);  // display on/off
	lcd_write_command(0x08);
	pll_delay_us(100);
	lcd_write_command(0x00);  // display clear
	lcd_write_command(0x01);
	pll_delay_ms(4);
	lcd_write_command(0x00);  // entry mode set
	lcd_write_command(0x06);
	pll_delay_us(100);
	lcd_write_command(0x00);  // display on
	lcd_write_command(0x0C);
	// clear LCD before use, sets cursor to (0, 0)
	lcd_clear();  
	// load custom characters to CGRAM
	lcd_custom_character(customDivide, 7);
	lcd_custom_character(dith1, 0);
	lcd_custom_character(dith2, 1);
	lcd_custom_character(dith3, 2);
	lcd_custom_character(lock, 6);
}

void lcd_load(void) {	
	// quickly pulse enable pin to load data into LCD					
	EN = 0x04;
	pll_delay_ms(1);
	EN = 0x00;
	pll_delay_ms(1);
}

void lcd_write_command(unsigned char c) {
	// select command register
	RS = 0x00;
	// output command to data pins, b3..0
	DATA = c & 0x0F;
	lcd_load();
}

void lcd_write_data(char c) {
	// select data register
	RS = 0x08;
	// output data to data pins, mask unused bits
	DATA = c & 0x0F;
	lcd_load();
}

void lcd_clear(void) {
	// select command register
	RS = 0x00;
	// 0x10 = clear lcd command
	lcd_write_command(0x00);
	lcd_write_command(0x01);
}

void lcd_print_char(char c) {
	// write char to data register, b7..4 first
	lcd_write_data(c >> 4);
	lcd_write_data(c);
}

void lcd_print_char_value(char c) {
	int i = 0;  // loop iterator
	// print '0b' prefix
	lcd_print_char('0');
	lcd_print_char('b');
	// loop through char, printing value of each bit
	for(i = 0; i < 8; i++) {
		if((c >> (7 - i)) & 1) {
			lcd_print_char('1');
		}
		else {
			lcd_print_char('0');
		}
	}
}

void lcd_print_string(char s[]) {
	int i;  // loop iterator
	// iterate through string, REF: 
	// https://stackoverflow.com/questions/3213827/how-to-iterate-over-a-string-in-c
	for (i = 0; s[i] !=0; i++) {
		// wrap to new line if 16 characters is exceded				
		if(i == 16) {
			lcd_goto(0x01, 0x00);
		}
		// print char from char array/string
		lcd_print_char(s[i]);
	}
}

void lcd_goto(unsigned char l, unsigned char p) {
	// 0b8l00 pppp	: 8 = select DDRAM address, l = line, p = position
	lcd_write_command(0x08 + (l << 2));
	lcd_write_command(p);
}

void debug_mess(char str[]) {
	// toggle messages with DEBUG_ON in header
	if(DEBUG_ON) {
		// briefly display input string with D: prefix
		lcd_clear();
		pll_delay_ms(100);
		lcd_print_string("D: ");
		lcd_print_string(str);
		pll_delay_ms(500);
	}
}

//https://circuitdigest.com/microcontroller-projects/custom-characters-on-lcd-using-pic16f877a
void lcd_custom_character(unsigned char* character, char location) {
	int i = 0;  // loop iterator
	// set CGRAM address of custom character
	char store = (0x40 + location*8);
	lcd_write_command(store >> 4);
	lcd_write_command(store);
	// write each line of custom character to CGRAM
	for(i = 0; i < 8; i++) {
		lcd_print_char(character[i]);
	}
	// return to home
	lcd_write_command(0x00);
	lcd_write_command(0x02);
}

void lcd_splash_animation(void) {
	int i, character;
	
	lcd_clear();
	pll_delay_ms(50);
	for(character = 2; character >= 0; character--) {
		lcd_goto(0,0);
		for(i = 0; i < 16; i++) {
			lcd_print_char(character);
		}
		lcd_goto(1,0);
		for(i = 0; i < 16; i++) {
			lcd_print_char(character);
		}
		pll_delay_ms(25);
	}
	pll_delay_ms(100);
	for(character = 0; character < 3; character++) {
		lcd_goto(0,0);
		for(i = 0; i < 16; i++) {
			lcd_print_char(character);
		}
		lcd_goto(1,0);
		for(i = 0; i < 16; i++) {
			lcd_print_char(character);
		}
		pll_delay_ms(25);
	}
	lcd_clear();
	pll_delay_ms(250);
	for(i = 0; i < 16; i++) {
			lcd_print_char(0);
			pll_delay_ms(10);
	}
	lcd_goto(1,0);
	for(i = 15; i >= 0; i--) {
		lcd_goto(1, i);
			lcd_print_char(0);
			pll_delay_ms(10);
	}
	for(i = 0; i < 16; i++) {
		 lcd_goto(0, i);
			lcd_print_char(' ');
			pll_delay_ms(10);
	}
	lcd_goto(1,0);
	for(i = 15; i >= 0; i--) {
		 lcd_goto(1, i);
			lcd_print_char(' ');
			pll_delay_ms(10);
	}
	
	pll_delay_ms(200);
	lcd_print_string("ELEC3662 Calc!");
	pll_delay_ms(1000);
	lcd_clear();
	
	
}