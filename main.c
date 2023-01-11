// Library/driver inclusions
#include "KeyPad.h"
#include "LCD.h"
#include "PLL.h"
#include "Utility.h"
#include "ShuntingYard.h"
#include "Flash.h"
// Constant definitions
#define SCREEN_WIDTH 16
#define OUTPUT_BUFFER_SIZE 16
#define INPUT_BUFFER_SIZE 100
//Function declarations
char get_input(void);
void clear_calculator(void);
void display(void);
void calculate(char str[]);
int enter_pw(void);
int update_pw(void);
// Variable definitions
char input;             // stores most recent input
char inputBuffer[100];  // stores all input until calculation
int inputIndex = 0;     // tracks inputBuffer position
char outputBuffer[16];  // chars for output display

char prevInput;         // stores previous input
unsigned int shiftFlag = 0;  // tracks shift status

Token res = {0.0, -1};  // stores calculation result
Stack tokenStack;       // stack of tokens for calculation
Stack operatorStack;    // stack of operations for calculation
Stack outputStack;      // stack of postfix sorted tokens
Stack resultStack;      // stack used in calculation

int errFlag = 0;        // syntax/math error tracking flag
char mathErr[] = "MATH_ERR";
char syntaxErr[] = "SYNTAX_ERR";

int main(void) {
	// Initialise peripherals
	pll_init();  // pll must be initialised first
	lcd_init();  // lcd initialisation
	keypad_init();  // keypad initialisation
	flash_init();  // flash initialisation
	
	lcd_splash_animation();  // show splash screen
	// call enter_pw while password is incorrect
	while(!enter_pw()) {pll_delay_ms(500);};
	pll_delay_ms(500);
	lcd_clear();
	// main program loop
	while(1) {
		// get user input
		input = get_input();
		// perform actions based on returned char
		switch(input) {  
			case EQUALS: {
				calculate(inputBuffer);
				break;
			}
			case PASSWORD: {
				update_pw();
				clear_calculator();
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
			// insert char into buffer then increment index if integer
			default: {
				inputBuffer[inputIndex] = input;
				inputIndex++;
			}
		}
		// update display of inputBuffer
		display();  
		pll_delay_ms(10);
	};
	
}
/**
 * Get filtered input based on previous input, shift state
*/
char get_input(void) {
	char out = 'X';  //'no entry' symbol
	shiftFlag = 0;  // reset flags
	prevInput = 0;
	// loop until different input is received
	// read keypad each loop
	// if statement checks for a new keypress
	while(1) {
		char i = keypad_read();
		if(i != prevInput && i != 'X' && inputIndex < INPUT_BUFFER_SIZE) {
			// update old input
			prevInput = i;  
			// toggle shift flag if shift pressed
			if(i == SHIFT) {
				shiftFlag = !shiftFlag;  // toggle shift flag
			}
			else {
				// set output based on input + shift flag
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
					case EQUALS: {
						switch(shiftFlag) {
							case 1: {
								out = PASSWORD;
								break;
							}
							default: {
								out = EQUALS;
								break;
							}
						}
						break;
					}
					// default for inputs 0-9, return integer
					default: {  
						out = i;
						break;
					}
				}
			}
		}
		//return once key is released
		else if (i == 'X' && out != 'X') {  
			return out;
		}
	}
}
/**
 * Clear I/O buffers and display
*/
void clear_calculator(void) {
	int i;  // loop iterator
	// loop through input buffer, clearing characters
	for(i = 0; i < INPUT_BUFFER_SIZE; i++) {
		inputBuffer[i] = ' ';
	}
	// loop through output buffer, clearing characters
	for(i = 0; i < OUTPUT_BUFFER_SIZE; i++) {
		outputBuffer[i] = ' ';
	}
	// reset input index
	inputIndex = 0;
	// update display (clears display)
	display();
}
/**
 * Update LCD display with I/O buffer contents
*/
void display(void) {
	int i = 0;  // loop iterator
	lcd_clear();
	// if user input wont fit on screen
	if(inputIndex>SCREEN_WIDTH) {
		// print last 16 characters of buffer
		for(i = inputIndex-SCREEN_WIDTH; i < inputIndex; i++) {
			lcd_print_char(inputBuffer[i]);
		}
	// if user input fits on screen
	} else {
		// print input
		for(i = 0; i < inputIndex; i++) {
			lcd_print_char(inputBuffer[i]);
		}
	}
	// print output buffer on line 2 of LCD
	lcd_goto(1,0);
	lcd_print_string(outputBuffer);
}
/**
 * Convert input string to postfix notation using shunting yard algorithm
 * Perform calculation on equation
*/
void calculate(char str[]) {
	lcd_clear();
	// add null terminator to end of input buffer
	inputBuffer[inputIndex] = '\0';
	// tokenise input buffer, storing any syntax errors
	errFlag = tokenise(&tokenStack, str);
	// sort into postfix notation
	shunt(&tokenStack, &operatorStack, &outputStack);
	// calculate result, storing any math errors
	errFlag |= result(&outputStack, &resultStack);
	lcd_clear();
	// display error message or result based on error flag
	lcd_goto(1,0);
	switch(errFlag) {
		case ERR: {
			copy_string(outputBuffer, syntaxErr);
			break;
		}
		case NAN: {
			copy_string(outputBuffer, mathErr);
			break;
		}
		default: {
			double_to_string(resultStack.stack[0].value, outputBuffer, 6);
			lcd_print_string(outputBuffer);
			break;
		}
	}
}
/**
 * Read saved password from flash memory and compare to user input
*/
int enter_pw(void) {
	int i;  // loop iterator
	unsigned long entry = 0x00;  // user input
	unsigned long pw;  // saved password
	// get saved password from flash memory
	flash_read(&pw);  
	// display prompt on screen
	lcd_clear();
	lcd_print_string("ENTER PW:");
	lcd_goto(1,0);
	lcd_print_char(6);
	//get 4 character input from user and store in unsigned long
	for(i = 0; i < 4; i++) {
		entry = (entry << 8) + get_input();
		lcd_print_char('*');
	}
	lcd_goto(1,1);
	// return true if passwords match, false if not
	if(entry == pw) {
		lcd_print_string("UNLOCKED");
		return 1;
	}
	else {
		lcd_print_string("LOCKED");
		return 0;
	}
}
/**
 * Get user input for new password and write to flash memory
*/
int update_pw(void) {
	int i;  // loop iterator
	unsigned long newPW = 0x00;  // long to store new pasword
	// display prompt
	lcd_clear();
	lcd_print_char(6);
	lcd_print_string("    UPDATE    ");
	lcd_print_char(6);
	lcd_goto(1,0);
	lcd_print_char(6);
	lcd_print_string("   PASSWORD   ");
	lcd_print_char(6);
	pll_delay_ms(1000);
	// enter current password correctly to update
	while(!enter_pw()) {pll_delay_ms(500);}
	pll_delay_ms(500);
	// display prompt
	lcd_clear();
	lcd_print_string("ENTER NEW PW");
	lcd_goto(1,0);
	lcd_print_char(6);
	// get 4 character input to set as new password
	for(i = 0; i < 4; i++) {
		newPW = (newPW << 8) + get_input();
		lcd_print_char('*');
	}
	// return true if flash is written properly
	lcd_goto(1,1);
	lcd_print_string("UPDATED");
	pll_delay_ms(500);
	i = flash_write(newPW);
	return i + 1;
}