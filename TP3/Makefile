# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -g

# Targets
TARGET = TP3
SOURCES = TP3.c TP3Functions.c ../TP1/TP1Functions.c
OBJECTS = $(SOURCES:.c=.o)

# Default target
all: $(TARGET)

# Linking the executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

# Rule for building the object files
%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean targets
clean:
	rm -f $(OBJECTS) $(TARGET)