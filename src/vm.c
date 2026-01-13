#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/termios.h>
#include <sys/mman.h>

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