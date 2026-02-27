CC = g++
CFLAGS = -std=c++23 -ggdb

BIN = bin
SRC = src
INC = inc

LIBRARIES = -I.
EXECUTABLE = main

all: $(BIN)/$(EXECUTABLE)

run : clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CC) $(CFLAGS) $(LIBRARIES) $^ -o $@ $(LIBRARIES)

clean:
	-rm $(BIN)/*