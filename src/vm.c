#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/termios.h>
#include <sys/mman.h>

/* Keyboard Input Handling */

static struct termios originalTermios; // Original Terminal State

void disableInputBuffering(void)
{
    tcgetattr(STDIN_FILENO, &originalTermios);

    struct termios newTermios = originalTermios;
    newTermios.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSANOW, &newTermios);
}

void restoreInputBuffering(void)
{
    tcsetattr(STDIN_FILENO, TCSANOW, &originalTermios);
}

uint16_t checkKey(void)
{
    fd_set readFds;
    FD_ZERO(&readFds);
    FD_SET(STDIN_FILENO, &readFds);

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;

    return select(1, &readFds, NULL, NULL, &timeout) != 0;
}

/* Restore Settings on Signal */
void handleInterrupt(int signal){
    restoreInputBuffering();
    printf("\n");
    exit(-2);
}

#include "vm.h"

uint16_t swap16(uint16_t addr){
    return (addr << 8) | (addr >> 8);
}

void readImageFile(FILE* image){
    // First 16 Bits --> Specifies Program Start 
    uint16_t originAddr;
    fread(&originAddr, sizeof(originAddr), 1, image);
    originAddr = swap16(originAddr);

    // Max File Size Known --> One fread
    uint16_t maxRead = MEMORY_MAX - originAddr;

    // p --> Origin Offset Inside Memory Array
    uint16_t* p = memory + originAddr;
    size_t read = fread(p, sizeof(uint16_t), maxRead, image);

    // LC3 Programs = Big-Endian --> Swap To Little-Endian
    while (read > 0){
        *p = swap16(*p);
        ++p;
        --read;
    }
}

int readImage(const char* path){
    FILE* image = fopen(path, "rb");

    if (!image){
        return 0;
    }

    readImageFile(image);
    fclose(image);
    return 1;
}

void memoryWrite(uint16_t addr, uint16_t val){
    memory[addr] = val;
}

uint16_t memoryRead(uint16_t addr){
    if(addr == MR_KBSR){
        if (checkKey()){
            memory[MR_KBSR] = (1 << 15);
            memory[MR_KBDR] = getchar();
        }
        else{
            memory[MR_KBSR] = 0;
        }
    }

    return memory[addr];
}

uint16_t signExtend(uint16_t x, int bitCount){
    if ((x >> (bitCount - 1)) & 1){
        x |= (0xFFFF << bitCount);
    }
}

/* Updating Condition Flags */
void updateFlags(uint16_t r){
    if (reg[r] == 0){
        reg[REG_COND] = FL_ZRO;
    }
    else if (reg[r] >> 15){ // Most Significant Bit == 1 --> Negative Sign
        reg[REG_COND] = FL_NEG;
    }
    else{
        reg[REG_COND] = FL_POS;
    }
}

int main(int argc, const char* argv[]){
    /* Handling Improper/Broken Usage */
    if (argc < 2){
        printf("Usage: vm [image-file1] ...\n");
        exit(2);
    }

    for (int i = 1; i < argc; ++i){
        if (!readImage(argv[i])){
            printf("Image %s Failed to Load\n", argv[i]);
            exit(1);
        }
    }

    /* Terminal Input Handling */
    signal(SIGINT, handleInterrupt);
    disableInputBuffering();
    
    /* Conditional Flag Set To Z Flag */
    reg[REG_COND] = FL_ZRO;

    enum { PC_START = 0x3000 }; /* Default Program Counter Start */
    reg[REG_PC] = PC_START;

    int currRunning = 1;

    while (currRunning){

        /* Get Instruction & OP Code */
        uint16_t instruction = memoryRead(reg[REG_PC]++);
        uint16_t opCode = instruction >> 12;

        switch (opCode){
            case OP_BR:
            {
                /* Branch */
                /* Check Condition Flags & Update PC If Condition Met */
                break;
            }

            case OP_ADD:
            {
                /* Add */
                /* reg[dr] = reg[sr1] + (reg[sr2] or imm5) */

                uint16_t r0 = (instruction >> 9) & 0x7; // dr
                uint16_t r1 = (instruction >> 6) & 0x7; // sr1

                uint16_t immFlag = (instruction >> 5) & 0x1; // Immediate Mode Check
                

                if (immFlag){
                    uint16_t imm5 = signExtend(instruction & 0x1F, 5);
                    reg[r0] = reg[r1] + imm5;
                }
                else{
                    uint16_t r2 = instruction & 0x7; // sr2
                    reg[r0] = reg[r1] + reg[r2];
                }

                updateFlags(r0);

                break;
            }

            case OP_LD:
            {
                /* Load */
                /* reg[dr] = mem[PC + offset] */
                break;
            }

            case OP_ST:
            {
                /* Store */
                /* mem[PC + offset] = reg[sr] */
                break;
            }

            case OP_JSR:
            {
                /* Jump to Subroutine */
                /* Save PC in R7, Then Jump */
                break;
            }

            case OP_AND:
            {
                /* Bitwise AND */
                /* reg[dr] = reg[sr1] & (reg[sr2] or imm5) */
                break;
            }

            case OP_LDR:
            {
                /* Load Register */
                /* reg[dr] = mem[reg[base] + offset] */
                break;
            }

            case OP_STR:
            {
                /* Store Register */
                /* mem[reg[base] + offset] = reg[sr] */
                break;
            }

            case OP_RTI:
            {
                /* Return From Interrupt (Unused) */
                break;
            }

            case OP_NOT:
            {
                /* Bitwise NOT */
                /* reg[dr] = ~reg[sr] */
                break;
            }

            case OP_LDI:
            {
                /* Load Indirect */
                /* reg[dr] = mem[mem[PC + offset]] */
                break;
            }

            case OP_STI:
            {
                /* Store Indirect */
                /* mem[mem[PC + offset]] = reg[sr] */
                break;
            }

            case OP_JMP:
            {
                /* Jump */
                /* PC = reg[base] */
                break;
            }

            case OP_RES:
            {
                /* Reserved (Unused) */
                break;
            }

            case OP_LEA:
            {
                /* Load Effective Address */
                /* reg[dr] = PC + offset */
                break;
            }

            case OP_TRAP:
            {
                /* Trap (System Call) */
                /* Execute Trap Routine */
                break;
            }

            default:
            {
                abort();
                break;
            }
        }
        
    }
    restoreInputBuffering();
}