CC = gcc
CFLAGS = -std=c2x -ggdb

BIN = bin
SRC = src
INC = inc

OBJ = src/obj

LIBRARIES =
EXECUTABLE = main

all: $(BIN)/$(EXECUTABLE)

run : clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.c
	$(CC) $(CFLAGS) -I. $^ -o $@ $(LIBRARIES)

clean:
	-rm $(BIN)/*