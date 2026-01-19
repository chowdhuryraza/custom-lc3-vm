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

void memoryWrite(uint16_t addr, uint16_t val);
uint16_t memoryRead(uint16_t addr);

uint16_t signExtend(uint16_t x, int bitCount);
void updateFlags(uint16_t r);


/* Trap Vectors (System Call Identifiers) */
typedef enum
{
    TRAP_GETC  = 0x20, /* Read Single Character From Keyboard (Not Echoed) */
    TRAP_OUT   = 0x21, /* Output Character in R0 To Console */
    TRAP_PUTS  = 0x22, /* Output Null-Terminated String (One Character Per Word) */
    TRAP_IN    = 0x23, /* Read Single Character From Keyboard (Echoed) */
    TRAP_PUTSP = 0x24, /* Output String Packed Two Characters Per Word */
    TRAP_HALT  = 0x25  /* Halt Program Execution */
} TrapVector;



#endif