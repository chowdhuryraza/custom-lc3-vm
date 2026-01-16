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
    


    /* Conditional Flag Set To Z Flag */
    reg[REG_COND] = FL_ZRO;

    enum { PC_START = 0x3000 }; /* Default Program Counter Start */
    reg[REG_PC] = PC_START;

    int currRunning = 1;

    while (currRunning){
        
    }

}