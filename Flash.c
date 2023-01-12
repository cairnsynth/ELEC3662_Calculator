#include "Flash.h"
// store WRKEY value based on BOOTCFG
static unsigned int wrKey = 0;

void flash_init(void) {
	//If 'KEY' bit in BOOTCFG register is set
	if(SYSCTL_BOOTCFG_R & 0x10) {  
		wrKey = 0xA442;  //Set correct WRKEY
	}
	else {
		wrKey = 0x71D5;  //Set correct WRKEY
	}
}

int flash_erase(int blockCount) {
	int i;
	// varify wrKey is loaded before proceding
	if(wrKey != 0xA442 || wrKey != 0x71D5) {
		return -1;
	}
	// iterate through blockCount
	for(i = 0; i < blockCount; i++) {
		// mask reserved bits in FMA (b31..18)
		FLASH_FMA_R &= 0xFFFC0000;  
		// set address to erase in FMA
		FLASH_FMA_R |= ((unsigned long)FLASH_BASE_ADDR) + (i*1024);
		// write WRKEY and erase bit to FMC, must be set simultaneously
		FLASH_FMC_R = (wrKey << 16) | 0x02;
		// poll erase flag for completion
		while(FLASH_FMC_R & 0x02) {}
	}
	return 0;
}

int flash_write(const unsigned long data) {
	// get number of blocks to erase
	int blockCount = (((sizeof(unsigned long))/1024) + 1);
	// check WRKEY is set before proceding
	if(wrKey != 0xA442 || wrKey != 0x71D5) {
		return -1;
	}
	// blocks must be erased before being written
	// write can only clear bits so all must be set before write
	flash_erase(blockCount);
	// write input data to FMD
	FLASH_FMD_R = (data);	
	// mask reserved bits in FMA
	FLASH_FMA_R &= 0xFFFC0000;
	// set flash address
	FLASH_FMA_R |= ((unsigned long)FLASH_BASE_ADDR);
	// write WRKEY and write bit to FMC, must be set simultaneously
	FLASH_FMC_R = (wrKey << 16) | 0x01;
	// poll write bit for completion
	while(FLASH_FMC_R & 0x01) {}
	return 0;
}

void flash_read(void* out) {
	// return data stored at flash base address
	((unsigned long*)out)[0] = FLASH_BASE_ADDR[0];
}

unsigned long flash_previous(void) {
	// previous write is stored in FMD register
	return FLASH_FMD_R;
}