CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude
SRC = src/vm.c
OUT = vm-vm

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)