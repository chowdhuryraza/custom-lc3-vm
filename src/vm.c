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

int main(int argc, const char* argv[]){
    /* Handling Improper/Broken Usage */
    if (argc < 2){
        printf("Usage: vm [image-file1] ...\n");
        exit(2);
    }

    for (int i = 1; i < argc; ++i){
        if (!read_image(argv[i])){
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