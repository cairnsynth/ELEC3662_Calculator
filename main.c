#include "KeyPad.h"
#include "LCD.h"
#include "PLL.h"
#include "Utility.h"
#include "ShuntingYard.h"
#include <stdio.h>

#define SCREEN_WIDTH 16
#define OUTPUT_BUFFER_SIZE 16
#define INPUT_BUFFER_SIZE 100
#define MAX_INPUT_VALUES INPUT_BUFFER_SIZE/2

char get_input(void);
void clear_calculator(void);
void display(void);
void calculate(char str[]);

char input;
char inputBuffer[100];
int inputIndex = 0;
char outputBuffer[50];

char prevInput;
unsigned int shiftFlag = 0;
Token res = {0.0, -1};
Stack tokenStack;
Stack operatorStack;
Stack outputStack;
Stack resultStack;
int errFlag = 0;

char testStr[] = "1-99+3x10";
int looper = 0;

int main(void) {
	pll_init();																//pll must be initialised first as it is used in LCD initialisation
	lcd_init();																//lcd initialisation
	keypad_init();														//keypad initialisation
	lcd_print_string("Elec3662 Calc!");				//Display splash screen
	lcd_goto(1,0);
	lcd_print_string("SID: 201331120");
	pll_delay_ms(1000);												//Splash screen delay
	lcd_clear();															//Clear screen
	

	//calculate(testStr);
	pll_delay_ms(1000);
	
	
	while(1) {
		input = get_input();
		switch(input) {
			case EQUALS: {
				
				calculate(inputBuffer);
				break;
			}
			case CLEAR: {
				inputIndex -= 1;
				inputBuffer[inputIndex] = '\0';
				break;
			}
			case ALLCLEAR: {
				clear_calculator();
				break;
			}
			default: {
				inputBuffer[inputIndex] = input;
				inputIndex++;
			}
		}
		display();
		pll_delay_ms(10);
	};
	
}

char get_input(void) {
	char out = 'X';
	shiftFlag = 0;
	prevInput = 0;
	while(1) {
		char i = keypad_read();
		if(i != prevInput && i != 'X' && inputIndex < INPUT_BUFFER_SIZE) {
			prevInput = i;
			
			if(i == SHIFT) {
				shiftFlag = !shiftFlag;
			}
			else {
				switch(i) {
					case PLUS: {
						switch(shiftFlag) {
							case 1: out = TIMES;
								break;
							default: out = PLUS;
								break;
						}
						break;
					}
					case MINUS: {
						switch(shiftFlag) {
							case 1: out = DIVIDE;
								break;
							default: out = MINUS;
								break;
						}
						break;
					}
					case CLEAR: {
						switch(shiftFlag) {
							case 1: out = ALLCLEAR;
								break;
							default: out = CLEAR;
								break;
						}
						break;
					}
					case DECIMAL: {
						switch(shiftFlag) {
							case 1: out = EXPONENT;
								break;
							default: out = DECIMAL;
								break;
						}
						break;
					}
					default: {
						out = i;
						break;
					}
				}
				
				//shiftFlag = 0;
			}
		}
		else if (i == 'X' && out != 'X') {
			return out;
		}
	}
}

void clear_calculator(void) {
	int count;
	for(count = 0; count < INPUT_BUFFER_SIZE; count++) {
		inputBuffer[count] = ' ';
	}
	for(count = 0; count < OUTPUT_BUFFER_SIZE; count++) {
		outputBuffer[count] = ' ';
	}
	inputIndex = 0;
	display();
}

void display(void) {
	int count = 0;
	lcd_clear();
	lcd_goto(0, 0);
	if(inputIndex>SCREEN_WIDTH) {
		for(count = inputIndex-SCREEN_WIDTH; count < inputIndex; count++) {
			lcd_print_char(inputBuffer[count]);
		}
	} else {
		for(count = 0; count < inputIndex; count++) {
			lcd_print_char(inputBuffer[count]);
		}
	}
	lcd_goto(1,0);
	lcd_print_string(outputBuffer);
}

void calculate(char str[]) {
	lcd_clear();
	inputBuffer[inputIndex] = '\0';
	errFlag = tokenise(&tokenStack, str);
	shunt(&tokenStack, &operatorStack, &outputStack);
	errFlag |= result(&outputStack, &resultStack);
	
	lcd_clear();
	lcd_goto(1,0);
	switch(errFlag) {
		case ERR: {
			lcd_print_string("SYNTAX ERR");
			break;
		}
		case NAN: {
			lcd_print_string("MATH ERR");
			break;
		}
		default: {
			double_to_string(resultStack.stack[0].value, outputBuffer, 6);
			lcd_print_string(outputBuffer);
			break;
		}
	}
}

