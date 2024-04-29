CC = gcc
CFLAGS = -Wall -Wextra -Werror -lncurses -lm
DEPS = engine/engine.h utils/math.h utils/data_structures.h
OBJ = engine/engine.o utils/math.o utils/data_structures.o
TARGET = main

%.o: %.c $(DEPS)
	$(CC) -c -g -o $@ $< $(CFLAGS)

$(TARGET):  main.o $(OBJ)
	$(CC) -g -o $@ $^ $(CFLAGS)

test: tests/tests.o $(OBJ)
	$(CC) -g -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(OBJ) $(TARGET)