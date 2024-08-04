# Compiler definition
CC = gcc
# Compiler flags: compiler warnings and extra warnings
# add include directory
CFLAGS = -Wall -Wextra -Iinclude

# Source and object files
SRC = src/main.c src/server.c
OBJ = $(SRC:.c=.o)

# Name of executable
TARGET = http_server

# Makes on target
all: $(TARGET)

# Compiles object files to finale executable
$(TARGET): $(OBJ)
	$(CC) -o $@ $^

# Compiles .c files to .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target to remove built files
clean:
	rm -f $(OBJ) $(TARGET)

# Declare phony targets
.PHONY: all clean
