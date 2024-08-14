# Compiler definition
CC = gcc
# Compiler flags: compiler warnings and extra warnings
CFLAGS = -Wall -Wextra -Iinclude -Iunity -Iutils
LDFLAGS = -lws2_32

# Source and object files
SRC = src/main.c src/server.c utils/base64_decode.c
OBJ = $(SRC:.c=.o)

# Name of executable
TARGET = http_server

# Test files
TEST_SRC = test/test_server.c src/server.c utils/base64_decode.c unity/unity.c
TEST_OBJ = $(TEST_SRC:.c=.o)
TEST_TARGET = test_run

# Default target
all: $(TARGET)

# Link final executable
$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

# Compile .c files to .o files (source files)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test_%.o: test/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile .c files to .o files (test files)
$(TEST_OBJ): %.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Target to build the test runner
$(TEST_TARGET): $(TEST_OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

# Run tests
test: $(TEST_TARGET)
	./$(TEST_TARGET)

# Clean target to remove built files
clean:
	rm -f $(OBJ) $(TARGET) $(TEST_OBJ) $(TEST_TARGET)

# Declare phony targets
.PHONY: all clean test
