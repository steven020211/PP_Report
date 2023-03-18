# Compiler and compiler flags
CC=g++
CFLAGS=-c -Wall

# Source files
SRCS=$(wildcard *.cpp)

# Object files
OBJS=$(patsubst %.cpp,%.o,$(SRCS))

# Target executable
TARGET=image_adjustment

# Build rules
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $^ -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

# Phony targets
.PHONY: clean all

# Clean rule
clean:
	rm -f *.o $(TARGET)
