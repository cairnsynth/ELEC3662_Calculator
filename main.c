#include "KeyPad.h"
#include "LCD.h"
#include "PLL.h"
#include "Utility.h"

#define SCREEN_WIDTH 16
#define OUTPUT_BUFFER_SIZE 16
#define INPUT_BUFFER_SIZE 100
#define MAX_INPUT_VALUES INPUT_BUFFER_SIZE/2

void get_input(void);
void calculate(void);
void clear_calculator(void);
void display(void);

float inputValues[MAX_INPUT_VALUES];
char operations[MAX_INPUT_VALUES];

char inputBuffer[INPUT_BUFFER_SIZE];
int inputIndex = 0;
char outputBuffer[OUTPUT_BUFFER_SIZE];

char prevInput;

unsigned int shiftFlag = 0;

int main(void) {
	pll_init();																//pll must be initialised first as it is used in LCD initialisation
	lcd_init();																//lcd initialisation
	keypad_init();														//keypad initialisation
	lcd_print_string("ELEC3662 Calc!");				//Display splash screen
	lcd_goto(1,0);
	lcd_print_string("SID: 201331120");
	pll_delay_ms(1000);												//Splash screen delay
	lcd_clear();															//Clear screen
	
	clear_calculator();
	
	while(1) {
		get_input();
		pll_delay_ms(100);
	};
	
}

void get_input(void) {
	char input = keypad_read();
	if(input != prevInput && input != 'X' && inputIndex < INPUT_BUFFER_SIZE) {
		switch(input) {
			case '=':
				calculate();
				break;
			case '+': {
				switch(shiftFlag) {
					case 1:
						inputBuffer[inputIndex] = 'x';
						break;
					default:
						inputBuffer[inputIndex] = '+';
						break;
				}
				inputIndex++;
				shiftFlag = 0;
				break;
			}
			case '-': {
				switch(shiftFlag) {
					case 1:
						inputBuffer[inputIndex] = '/';
						break;
					default:
						inputBuffer[inputIndex] = '-';
						break;
				}
				inputIndex++;
				shiftFlag = 0;
				break;
			}
			case 'S':
				shiftFlag = !shiftFlag;
				break;
			case 'C': {
				switch(shiftFlag) {
					case 1:
						inputIndex = 0;
						break;
					default:
						inputBuffer[inputIndex-1] = ' ';
					inputIndex--;
						break;
				}
				shiftFlag = 0;
				break;
			}
			default: {
				inputBuffer[inputIndex] = input;
				inputIndex++;
				break;
			}
		}
		display();
	}
	prevInput = input;
	
}

void calculate(void) {
	int i;
	int inputCount = 0, valCount = 0, operatorCount = 0, decimalFlag=0, decimalPlace=1;
	double tempVal = 0.0, calcA = 0.0, calcB = 0.0;
	
	for(inputCount = 0; inputCount < inputIndex; inputCount++) {
		if(inputBuffer[inputCount] <= 0x39 && inputBuffer[inputCount] >= 0x30) {
			add_digit(&tempVal, inputBuffer[inputCount] - '0', decimalFlag, &decimalPlace);
		} else {
			if(inputBuffer[inputCount] == '.') {
				decimalFlag = 1;
			} else {
				inputValues[valCount] = tempVal;
				valCount++;
				operations[operatorCount] = inputBuffer[inputCount];
				operatorCount++;
				tempVal = 0.0;
				decimalFlag = 0;
				decimalPlace = 1;
			}
			
		}
	}
	inputValues[valCount] = tempVal;
	valCount++;
	/*
	//DEBUG: SHOW ALL INPUT VALUES
	for(i = 0; i < valCount; i++) {
		double_to_string(inputValues[i], outputBuffer, 3);
		display();
		pll_delay_ms(500);
	}
	pll_delay_ms(1000);
	
	//DEBUG END
	*/
	calcA = inputValues[0];
	for(i = 1; i < valCount; i++) {
		calcB = inputValues[i];
		switch(operations[i-1]) {
			case '+': {
				calcA += calcB;
				break;
			}
			case '-': {
				calcA -= calcB;
				break;
			}
			case '*': {
				calcA *= calcB;
				break;
			}
			case '/': {
				calcA /= calcB;
				break;
			}
			/*
			//DEBUG: SHOW CALCULATIONS
			double_to_string(calcA, outputBuffer, 5);
			display();
			pll_delay_ms(500);
			double_to_string(calcB, outputBuffer, 5);
			display();
			pll_delay_ms(500);
			//DEBUG END
			*/
		}
	}
	double_to_string(calcA, outputBuffer, 5);
	display();
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
	lcd_goto(1, 0);
	for(count = 0; count < SCREEN_WIDTH; count++) {
		lcd_print_char(outputBuffer[count]);
	}
	lcd_goto(0, 0);
}
