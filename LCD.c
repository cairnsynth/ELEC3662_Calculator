#include "LCD.h"
#include "PLL.h"

unsigned char customDivide[8] = {0x0,0x4,0x0,0x1f,0x0,0x4,0x0,0x0};
unsigned char dith1[8] = {0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f,0x1f};
unsigned char dith2[8] = {0x0a,0x17,0x0a,0x1d,0x0a,0x17,0x0a,0x1d};
unsigned char dith3[8] = {0x15,0x08,0x15,0x02,0x15,0x08,0x15,0x0a};
unsigned char snake1[8] = {0x0,0xe,0x15,0x1,0xf,0x12,0x0,0x0};
unsigned char snake2[8] = {0x0,0x0,0x1f,0xa,0x0,0x1f,0x0,0x0};
unsigned char snake3[8] = {0x0,0x0,0x7,0xa,0x10,0x1f,0x0,0x0};

void lcd_init(void) {												/*Initialise LCD*/
	volatile unsigned long delay;
	SYSCTL_RCG2_R |= 0x03;										//Clock enable for Port A and B
	delay = SYSCTL_RCG2_R;										//Wait for clock
	
																						/*Port Initialisation*/
	GPIO_PORTA_AMSEL_R &= 0x00;								//Disable analog function
	GPIO_PORTA_PCTL_R  &= 0x00000000;					//PCTL GPIO on Port A
	GPIO_PORTA_DIR_R   |= 0xFF;								//Set PA3-2 to output
	GPIO_PORTA_AFSEL_R &= 0x00;								//Disable Port A alt function
	GPIO_PORTA_PUR_R   &= 0x00;								//Disable pull-up resistors
	GPIO_PORTA_DEN_R   |= 0xFF;								//Enable digital I/O on PA3-2
	
	GPIO_PORTB_AMSEL_R &= 0x00;								//Disable analog function
	GPIO_PORTB_PCTL_R  &= 0x00000000;					//PCTL GPIO on Port B
	GPIO_PORTB_DIR_R   |= 0xFF;								//Set PB3-0 to output
	GPIO_PORTB_AFSEL_R &= 0x00;								//Disable Port B alt function
	GPIO_PORTB_PUR_R   &= 0x00;								//Disable pull-up resistors
	GPIO_PORTB_DEN_R   |= 0xFF;								//Enable digital I/O on PB3-0
	
	pll_delay_ms(100);												//Wait >40ms after Vcc rises above 4.5V
	lcd_write_command(0x03);									//"Function set" instruction
	pll_delay_ms(5);													//Wait >4.1ms
	lcd_write_command(0x03);									//"Function set" second instance
	pll_delay_100u();													//Wait >100us
	lcd_write_command(0x03);									//"Function set" third instance, results in LCD reset
	pll_delay_100u();
	lcd_write_command(0x02);									//Set 4 bit interface
	pll_delay_100u();
	lcd_write_command(0x02);									//Set number of lines and font
	lcd_write_command(0x08);
	pll_delay_100u();
	lcd_write_command(0x00);									//Display on/off
	lcd_write_command(0x80);
	pll_delay_100u();
	lcd_write_command(0x00);									//Display clear
	lcd_write_command(0x01);
	pll_delay_ms(4);
	lcd_write_command(0x00);									//Entry mode set
	lcd_write_command(0x06);
	pll_delay_100u();
	lcd_write_command(0x00);									//Display on
	lcd_write_command(0x0C);
	
	pll_delay_ms(500);												//Short delay
	lcd_clear();															//Clear LCD before use

	lcd_custom_character(customDivide, 7);
	lcd_custom_character(dith1, 0);
	lcd_custom_character(dith2, 1);
	lcd_custom_character(dith3, 2);
	lcd_custom_character(snake1, 3);
	lcd_custom_character(snake2, 4);
	lcd_custom_character(snake3, 5);
}

void lcd_en_pulse(void) {												/*Enable Pulse*/								
	EN = 0x04;																//Set enable pin high
	pll_delay_ms(1);													//Delay 1ms
	EN = 0x00;																//Set enable pin low
	pll_delay_ms(1);													//Delay 1ms
}

void lcd_write_command(unsigned char c) {		/*Write command to LCD*/
	RS = 0x00;																//Select command register
	DATA = c;																	//Output command to data bits (PB3-0)
	lcd_en_pulse();														//Pulse enable pin to write
}

void lcd_write_data(char c) {
	RS = 0x08;																//Set registet to data
	DATA = c & 0x0F;													//Set data to bits 3-0 of input, mask bits 7-4 (not strictly necessary as PB7-4 are not used)
	lcd_en_pulse();														//Pulse to write
}

void lcd_clear(void) {											/*Clear LCD*/
	RS = 0x00;																//Set register to command
	lcd_write_command(0x00);									//LCD clear command = 0x01
	lcd_write_command(0x01);									//Sent as 2x4 bit due to 4 bit operation
}

void lcd_print_char(char c) {								/*Print ASCII Character to LCD*/
	lcd_write_data(c >> 4);										//Set data bits to b7-4 of ASCII char
	lcd_write_data(c);									      //Set data bits to b3-0 of ASCII char
}

void lcd_print_char_literal(char c) {				/*Print char as its binary value (used for debugging)*/
	int i = 0;																//Iterator
	lcd_print_char('0');											//Print "0b" prefix
	lcd_print_char('b');
	for(i = 0; i < 8; i++) {									//Loop through char
		if((c >> (7 - i)) & 1) {								//If bit is 1
			lcd_print_char('1');									//Print 1
		}
		else {																	//Else print 0
			lcd_print_char('0');
		}
	}
}

void lcd_print_string(char s[]) {						/*Print char string to LCD*/
	int i;																		//Loop iterator
	for (i = 0; s[i] !=0; i++) {							//Iterate through string, ref: https://stackoverflow.com/questions/3213827/how-to-iterate-over-a-string-in-c
		if(i == 16) {														//If end of line is reached
			lcd_goto(0x01, 0x00);									//Wrap to new line
		}
		lcd_print_char(s[i]);										//Print char
	}
}

void lcd_goto(unsigned char l, unsigned char p) {					/*Goto position on LCD screen*/
	lcd_write_command(0x08 + (l << 2));											//l == 0: line 1, l == 1: line 2
	lcd_write_command(p);																		//position 0-16
}

void debug_mess(char str[]) {
	if(DEBUG_ON) {
		lcd_clear();
		pll_delay_ms(100);
		lcd_print_string("D: ");
		lcd_print_string(str);
		pll_delay_ms(500);
	}
}

//https://circuitdigest.com/microcontroller-projects/custom-characters-on-lcd-using-pic16f877a
void lcd_custom_character(unsigned char* character, char location) {
	int line = 0;
	char store = (0x40 + location*8);
	lcd_write_command(store >> 4);
	lcd_write_command(store);
	for(line = 0; line < 8; line++) {
		lcd_print_char(character[line]);
	}
	lcd_write_command(0x00);
	lcd_write_command(0x02);
}

void lcd_splash_animation(void) {
	int count, character;
	lcd_clear();
	pll_delay_ms(50);
	for(character = 2; character >= 0; character--) {
		lcd_goto(0,0);
		for(count = 0; count < 16; count++) {
			lcd_print_char(character);
		}
		lcd_goto(1,0);
		for(count = 0; count < 16; count++) {
			lcd_print_char(character);
		}
		pll_delay_ms(25);
	}
	pll_delay_ms(100);
	for(character = 0; character < 3; character++) {
		lcd_goto(0,0);
		for(count = 0; count < 16; count++) {
			lcd_print_char(character);
		}
		lcd_goto(1,0);
		for(count = 0; count < 16; count++) {
			lcd_print_char(character);
		}
		pll_delay_ms(25);
	}
	lcd_clear();
	pll_delay_ms(250);
	for(count = 0; count < 16; count++) {
			lcd_print_char(0);
			pll_delay_ms(10);
	}
	lcd_goto(1,0);
	for(count = 15; count >= 0; count--) {
		lcd_goto(1, count);
			lcd_print_char(0);
			pll_delay_ms(10);
	}
	for(count = 0; count < 16; count++) {
		 lcd_goto(0, count);
			lcd_print_char(' ');
			pll_delay_ms(10);
	}
	lcd_goto(1,0);
	for(count = 15; count >= 0; count--) {
		 lcd_goto(1, count);
			lcd_print_char(' ');
			pll_delay_ms(10);
	}
	lcd_goto(0,0);
	lcd_print_char(3);
	pll_delay_ms(100);
	lcd_clear();
	lcd_print_char(4);
	lcd_print_char(3);
	pll_delay_ms(100);
	lcd_clear();
	lcd_print_char(4);
	lcd_print_char(4);
	lcd_print_char(3);
	pll_delay_ms(100);
	lcd_clear();
	lcd_print_char(5);
	lcd_print_char(4);
	lcd_print_char(4);
	lcd_print_char(3);
	pll_delay_ms(100);
	lcd_clear();
	lcd_print_char('2');
	lcd_print_char(5);
	lcd_print_char(4);
	lcd_print_char(4);
	lcd_print_char(3);
	pll_delay_ms(100);
	lcd_clear();
	lcd_print_string("62");
	lcd_print_char(5);
	lcd_print_char(4);
	lcd_print_char(4);
	lcd_print_char(3);
	pll_delay_ms(100);
	lcd_clear();
	lcd_print_string("662");
	lcd_print_char(5);
	lcd_print_char(4);
	lcd_print_char(4);
	lcd_print_char(3);
	pll_delay_ms(100);
	lcd_clear();
	lcd_print_string("3662");
	lcd_print_char(5);
	lcd_print_char(4);
	lcd_print_char(4);
	lcd_print_char(3);
	pll_delay_ms(100);
	lcd_clear();
	lcd_print_string("C3662");
	lcd_print_char(5);
	lcd_print_char(4);
	lcd_print_char(4);
	lcd_print_char(3);
	pll_delay_ms(100);
	lcd_clear();
	lcd_print_string("EC3662");
	lcd_print_char(5);
	lcd_print_char(4);
	lcd_print_char(4);
	lcd_print_char(3);
	pll_delay_ms(100);
	lcd_clear();
	lcd_print_string("LEC3662");
	lcd_print_char(5);
	lcd_print_char(4);
	lcd_print_char(4);
	lcd_print_char(3);
	pll_delay_ms(100);
	lcd_clear();
	lcd_print_string("ELEC3662");
	lcd_print_char(5);
	lcd_print_char(4);
	lcd_print_char(4);
	lcd_print_char(3);
	pll_delay_ms(100);
	lcd_clear();
	lcd_print_string("ELEC3662 ");
	lcd_print_char(5);
	lcd_print_char(4);
	lcd_print_char(4);
	lcd_print_char(3);
	pll_delay_ms(100);
	lcd_clear();
	lcd_print_string("ELEC3662  ");
	lcd_print_char(5);
	lcd_print_char(4);
	lcd_print_char(4);
	lcd_print_char(3);
	pll_delay_ms(100);
	lcd_clear();
	lcd_print_string("ELEC3662   ");
	lcd_print_char(5);
	lcd_print_char(4);
	lcd_print_char(4);
	lcd_print_char(3);
	pll_delay_ms(100);
	lcd_clear();
	lcd_print_string("ELEC3662    ");
	lcd_print_char(5);
	lcd_print_char(4);
	lcd_print_char(4);
	lcd_print_char(3);
	pll_delay_ms(100);
	lcd_clear();
	lcd_print_string("ELEC3662    ");
	lcd_print_char(5);
	lcd_print_char(4);
	lcd_print_char(4);
	pll_delay_ms(100);
	lcd_clear();
	lcd_print_string("ELEC3662    ");
	lcd_print_char(5);
	lcd_print_char(4);
	pll_delay_ms(100);
	lcd_clear();
	lcd_print_string("ELEC3662    ");
	lcd_print_char(5);
	pll_delay_ms(100);
	lcd_clear();
	lcd_print_string("ELEC3662    ");
	pll_delay_ms(1000);
	lcd_clear();
}