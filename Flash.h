#ifndef FLASH_H
#define FLASH_H

/**
 * Flash.h
 * ------------------
 * AUTHOR: Luke Coles
 * 
 * ------------------
 * Library providing basic read and write access to the TM4C123GXL
 * on-chip flash memory.
 * ------------------
 * REFS:
 * 
 * TM4C123GXL datasheet (pg. 528 - 533)
 * 
 * Adapting code from jspicer-code, HAL_Flash.c,
 * <https://github.com/jspicer-code/Tiva-C-Embedded/blob/master/Experiment13-Flash/src/HAL_Flash.c>,
 * [accessed 12/01/2023]
 * 
*/

/* register address definitions */
#define SYSCTL_BOOTCFG_R  (*((volatile unsigned long *) 0x400FE1D0))
#define FLASH_FMA_R       (*((volatile unsigned long *) 0x400FD000))
#define FLASH_FMD_R       (*((volatile unsigned long *) 0x400FD004))
#define FLASH_FMC_R       (*((volatile unsigned long *) 0x400FD008))

// Total ROM size = 15.77kB ~= 16kB
// Total bits ~= 128000
// Flash address use = 0xFA0  (128000/32)
// Therefore FLASH_BASE_ADDR > 0xFA0 to avoid storage conflicts with code
#define FLASH_BASE_ADDR   (((volatile unsigned long *) 0x0020000))

/**
 * Initialise flash peripheral
 * Ensures WRKEY matches key bit in BOOTCFG register
*/
void flash_init(void);

/**
 * Erase data from flash memory
 * Erase can only set bits
 * Flash blocks must be erased before being written to store data correctly
 * 
 * blockCount: number of blocks to erase
*/
int flash_erase(int blockCount);

/**
 * Write data to flash memory
 * Memory must be erased before write as write can only clear bits
 * 
 * data: 32bits of data to write
*/
int flash_write(const unsigned long data);

/**
 * Read 32bits of data from flash
 * void* used to enable pointer to any data type
 * 
 * out: pointer to variable to store output data
*/
void flash_read(void* out);

/**
 * Return the data previously written to flash
 * Previous write is stored in the FMD register,
 * Reading this register gives value of previous write
 * Used for debugging
*/
unsigned long flash_previous(void);

#endif  // FLASH_H