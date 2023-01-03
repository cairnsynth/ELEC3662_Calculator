#include "KeyPad.h"
#include "LCD.h"
#include "PLL.h"
#include "Utility.h"

#define STRING_BUFFER_SIZE 50

int calculate_result(int a, int b);

char outputBuffer[STRING_BUFFER_SIZE];

char input;
int in[2] = {0,0};
int result = 0;
int inputFlag = 0;
int decimalFlag = 0;
int operationFlag = 0;
int calculateFlag = 0;

int main(void) {
	pll_init();																//pll must be initialised first as it is used in LCD initialisation
	lcd_init();																//lcd initialisation
	keypad_init();														//keypad initialisation
	lcd_print_string("ELEC3662 Calc!");				//Display splash screen
	lcd_goto(1,0);
	lcd_print_string("SID: 201331120");
	pll_delay_ms(1000);												//Splash screen delay
	lcd_clear();															//Clear screen
	
	//char foo[] = "Hello";
	//copy_string(outputBuffer, foo);
	
	//lcd_print_string(outputBuffer);
	
	
	
	while(1) {
		
		input = keypad_read();
		
		
		lcd_clear();
		lcd_print_char(input);
		
		
		pll_delay_ms(200);
		
	};
	
}

