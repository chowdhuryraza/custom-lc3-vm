#ifndef VM_H
#define VM_H

#include <stdint.h>
#include "lc3.h"

/* VM State */
extern uint16_t memory[MEMORY_MAX]; // Memory Array
extern uint16_t reg[REG_COUNT]; // Registers Array

/* VM Functions */
int readImage(const char* path);
void readImageFile(FILE* image);
uint16_t swap16(uint16_t addr);


#endif