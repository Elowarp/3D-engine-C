# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -g

# Source files
SRCS = main.c engine.c math.c

# Object files
OBJS = $(SRCS:.c=.o)

# Executable
TARGET = main

# Default target
all: $(TARGET)

tests: engine.o math.o
	$(CC) $(CFLAGS) $^ tests.c -o $@

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files into executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Clean up object files and executable
clean:
	rm -f $(OBJS) $(TARGET) tests tests.o
