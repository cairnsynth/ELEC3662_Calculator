#include "Keypad.h"
#include "PLL.h"

unsigned char key[4][4] = 	{{'1', '2', '3', '+'},
                            {'4', '5', '6', '-'},
                            {'7', '8', '9', '.'},
                            {'=', '0', 'C', 'S'}};

void keypad_init(void) {
	volatile unsigned long delay;
	SYSCTL_RCG2_R |= 0x18;  // enable clock for port d and e
	delay = SYSCTL_RCG2_R;	// wait for clock enable
	GPIO_PORTD_CR_R = 0xFF;	// enable changes to PD3-0
	GPIO_PORTD_AMSEL_R &= 0x00;  // disable analog function
	GPIO_PORTD_PCTL_R  &= 0x00000000;  // PCTL GPIO on PD3-0
	GPIO_PORTD_DIR_R   |= 0x00;  // set PD3-0 to input
	GPIO_PORTD_AFSEL_R &= 0x00;  // disable port d alt function
	GPIO_PORTD_PUR_R   &= 0x00;  // disable pull-up resistors
	GPIO_PORTD_PDR_R   |= 0x0F;  //Enable pull-down resistors
	GPIO_PORTD_DEN_R   |= 0x0F;  //Enable digital I/O on PD3-0
	
	GPIO_PORTE_AMSEL_R &= 0x00;
	GPIO_PORTE_PCTL_R  &= 0x00;
	GPIO_PORTE_DIR_R   |= 0xFF;
	GPIO_PORTE_AFSEL_R &= 0x00;
	GPIO_PORTE_PUR_R   &= 0x00;
	GPIO_PORTE_DEN_R   |= 0x0F;
	
}
/**
 * adapted from Microcontrollers Lab
 * REF:
 * Microcontrollers Lab, 4×4 Keypad Interfacing with TM4C123 Tiva Launchpad,
 * <https://microcontrollerslab.com/4x4-keypad-interfacing-tm4c123-tiva-launchpad-keil/>
 * [accessed 12/01/2023]
*/
unsigned char keypad_read(void) {
	int x=0, y=0;  // loop iterators
	// loop through columns
	for(x = 0; x < 4; x++) {
		// set column high
		GPIO_PORTE_DATA_R = (0x01 << x);
		pll_delay_us(20);
		// loop through rows
		for(y = 0; y < 4; y++) {
			// check row is high and return corresponding char
			if(GPIO_PORTD_DATA_R & (0x01 << y)) {
				return key[y][x];
			}
		}
	}
	// return X if no key is pressed
	return 'X';
}



