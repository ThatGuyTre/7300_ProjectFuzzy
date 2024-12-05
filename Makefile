# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c99 -g

# Source files
SRCS = fuzzy.c test.c sentence_search.c suffixtree.c

# Object files (automatically generated by replacing .c with .o)
OBJS = $(SRCS:.c=.o)

# Executable name
TARGET = fuzzy_test

# Default target
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile .c files into .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean
