CC = g++
CFLAGS = -std=c++23 -ggdb

BIN = bin
SRC = src
INC = inc

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