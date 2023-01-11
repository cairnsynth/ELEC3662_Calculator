#ifndef FLASH_H
#define FLASH_H

#define SYSCTL_BOOTCFG_R  (*((volatile unsigned long *) 0x400FE1D0))
#define FLASH_FMA_R				(*((volatile unsigned long *) 0x400FD000))
#define FLASH_FMD_R       (*((volatile unsigned long *) 0x400FD004))
#define FLASH_FMC_R       (*((volatile unsigned long *) 0x400FD008))

//Total ROM size = 15.77kB ~= 16kB
//Total bits ~= 128000
//Flash address use = 0xFA0  (128000/32)
//Therefore FLASH_BASE_ADDR > 0xFA0 to avoid storage conflicts with code
#define FLASH_BASE_ADDR       (((volatile unsigned long *) 0x0020000))

void flash_init(void);
int flash_erase(int blockCount);
int flash_write(const unsigned long data);
void flash_read(void* out);
unsigned long flash_previous(void);

#endif //FLASH_H