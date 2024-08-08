# Compiler definition
CC = gcc
# Compiler flags: compiler warnings and extra warnings
# add include directory
CFLAGS = -Wall -Wextra -Iinclude -Iunity

# Source and object files
SRC = src/main.c src/server.c
OBJ = $(SRC:.c=.o)

# Name of executable
TARGET = http_server

#Test files
TEST_SRC = test/test_server.c src/server.c unity/unity.c
TEST_OBJ = $(TEST_SRC:.c=.o)
TEST_TARGET = test_run

# Makes on target
all: $(TARGET)

# Compiles object files to finale executable
$(TARGET): $(OBJ)
	$(CC) -o $@ $^

# Compiles .c files to .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Target to build the test runner
$(TEST_TARGET): $(TEST_OBJ)
	$(CC) -o $@ $^
test: $(TEST_TARGET)
	./$(TEST_TARGET)

# Clean target to remove built files
clean:
	rm -f $(OBJ) $(TARGET) $(TEST_OBJ) $(TEST_TARGET)

# Declare phony targets
.PHONY: all clean
