#include "Flash.h"

//REF: https://github.com/jspicer-code/Tiva-C-Embedded/blob/master/Experiment13-Flash/src/HAL_Flash.c
//REF: https://www.ti.com/lit/ds/spms376e/spms376e.pdf?ts=1673398709820 (pg. 531)

static unsigned int wrKey = 0;

typedef union {
	double d;
	unsigned long l[2];
}writeVal;

void flash_init(void) {
	if(SYSCTL_BOOTCFG_R & 0x10) {  //If 'KEY' bit in BOOTCFG register is set
		wrKey = 0xA442;  //Set correct WRKEY
	}
	else {
		wrKey = 0x71D5;  //Set correct WRKEY
	}
}

int flash_erase(int blockCount) {
	int i;
	if(wrKey == 0) {				//If WRKEY has not been set
		return -1;																		//Exit
	}
	for(i = 0; i < blockCount; i++) {
		FLASH_FMA_R &= 0xFFFC0000;
		FLASH_FMA_R |= ((unsigned long)FLASH_BASE_ADDR) + (i*1024);
		FLASH_FMC_R = (wrKey << 16) | 0x2;
		while(FLASH_FMC_R & 0x2) {}
	}
	return 0;
}

int flash_write(const unsigned long data) {
	int blockCount = (((sizeof(unsigned long))/1024) + 1);
	if(wrKey == 0) {				//If WRKEY has not been set
		return -1;																		//Exit
	}
	flash_erase(blockCount);
	FLASH_FMD_R = (data);	
	FLASH_FMA_R &= 0xFFFC0000;
	FLASH_FMA_R |= ((unsigned long)FLASH_BASE_ADDR);
	FLASH_FMC_R = (wrKey << 16) | 0x1;
	while(FLASH_FMC_R & 0x1) {}
	return 0;
}

void flash_read(void* out) {
		((unsigned long*)out)[0] = FLASH_BASE_ADDR[0];
}

unsigned long flash_previous(void) {
	return FLASH_FMD_R;
}