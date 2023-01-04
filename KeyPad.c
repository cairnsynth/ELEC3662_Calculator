#include "Keypad.h"
#include "PLL.h"

unsigned char keyActual[4][4] = {{'1', '2', '3', 'A'},
																 {'4', '5', '6', 'B'},
																 {'7', '8', '9', 'C'},
																 {'*', '0', '#', 'D'}};

unsigned char key[4][4] = {{'1', '2', '3', '+'},
													 {'4', '5', '6', '-'},
													 {'7', '8', '9', '.'},
													 {'=', '0', 'C', 'S'}};

void keypad_init(void) {								/*Initialise keypad*/
	volatile unsigned long delay;
	SYSCTL_RCG2_R |= 0x18;					      /*Enable clock for port d and e*/
	delay = SYSCTL_RCG2_R;								/*Wait for clock enable*/
	GPIO_PORTD_CR_R = 0xFF;								/*Enable changes to PD3-0*/
	GPIO_PORTD_AMSEL_R &= 0x00;						/*Disable analog function*/
	GPIO_PORTD_PCTL_R  &= 0x00000000;				      /*PCTL GPIO on PD3-0*/
	GPIO_PORTD_DIR_R   |= 0x00;							/*Set PD3-0 to input*/
	GPIO_PORTD_AFSEL_R &= 0x00;						/*Disable port d alt function*/
	GPIO_PORTD_PUR_R   &= 0x00;							/*Disable pull-up resistors*/
	GPIO_PORTD_PDR_R   |= 0x0F;							/*Enable pull-down resistors*/
	GPIO_PORTD_DEN_R   |= 0x0F;							/*Enable digital I/O on PD3-0*/
	
	GPIO_PORTE_AMSEL_R &= 0x00;
	GPIO_PORTE_PCTL_R  &= 0x00;
	GPIO_PORTE_DIR_R   |= 0xFF;
	GPIO_PORTE_AFSEL_R &= 0x00;
	GPIO_PORTE_PUR_R   &= 0x00;
	GPIO_PORTE_DEN_R   |= 0x0F;
	
}

unsigned char keypad_read(void) {						/*Read button pressed and decode to char*/
	int x=0, y=0;															//Loop iterators
	for(x = 0; x < 4; x++) {									//Loop through columns
		GPIO_PORTE_DATA_R = (0x01 << x);						//Set column high
		pll_delay_us(20);
		for(y = 0; y < 4; y++) {								//Loop through rows
			if(GPIO_PORTD_DATA_R & (0x01 << y)) {		//If row input high
				return key[y][x];										//Return corresponding char
			}
		}
	}
	return 'X';
}



