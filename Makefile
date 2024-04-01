CC = gcc
CFLAGS = -Wall -Wextra -Werror -lncurses -lm
DEPS = engine/engine.h utils/math.h
OBJ = main.o engine/engine.o utils/math.o
TARGET = main

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -g -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(OBJ) $(TARGET)