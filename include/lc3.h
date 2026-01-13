#ifndef LC3_H
#define LC3_H

#include <stdint.h>

#define MEMORY_MAX (1 << 16) // Memory Storage Limit (128 KB)

typedef enum {
    /* General Purpose Registers */
    REG_R0 = 0,
    REG_R1,
    REG_R2,
    REG_R3,
    REG_R4,
    REG_R5,
    REG_R6,
    REG_R7,

    /* Program Counter */
    REG_PC,

    /* Condition Flags Register */
    REG_COND,

    REG_COUNT
} Register;


/* Opcodes */
enum
{
    OP_BR = 0, /* Branch */
    OP_ADD,    /* Add  */
    OP_LD,     /* Load */
    OP_ST,     /* Store */
    OP_JSR,    /* Jump Register */
    OP_AND,    /* Bitwise And */
    OP_LDR,    /* Load Register */
    OP_STR,    /* Store Register */
    OP_RTI,    /* Unused */
    OP_NOT,    /* Bitwise Not */
    OP_LDI,    /* Load Indirect */
    OP_STI,    /* Store Indirect */
    OP_JMP,    /* Jump */
    OP_RES,    /* Reserved (Unused) */
    OP_LEA,    /* Load Effective Address */
    OP_TRAP    /* Execute Trap */
};

/* Previous Calculation Condition Flags */
enum
{
    FL_POS = 1 << 0, /* Positive */
    FL_ZRO = 1 << 1, /* Zero */
    FL_NEG = 1 << 2, /* Negative */
};


#endif