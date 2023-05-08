# Compiler and compiler flags
CC=g++
CFLAGS=-c -Wall -O3
CFLAGS2=

# Source files
SRCS=$(filter-out image_adjustment%.cpp, $(wildcard *.cpp))
$(info test, SRCS= $(SRCS))
# Object files
#OBJS=$(patsubst %.cpp,%.o,$(SRCS))

# Target executable (serial)
TARGET_SERIAL=image_adjustment
SRC_SERIAL=image_adjustment.cpp $(SRCS)
OBJ_SERIAL=$(patsubst %.cpp,%.o,$(SRC_SERIAL))

# Target executable (pthread)
TARGET_PTHREAD=image_adjustment_pthread
SRC_PTHERAD=image_adjustment_pthread.cpp $(SRCS)
OBJ_PTHERAD=$(patsubst %.cpp,%.o,$(SRC_PTHERAD))

# Target executable (openmp)
TARGET_OPENMP=image_adjustment_openmp
SRC_OPENMP=image_adjustment_openmp.cpp $(SRCS)
OBJ_OPENMP=$(patsubst %.cpp,%.o,$(SRC_OPENMP))

# Target executable (cuda)
TARGET_CUDA=image_adjustment_cuda
SRC_CUDA=image_adjustment_cuda.cpp $(SRCS)
OBJ_CUDA=$(patsubst %.cpp,%.o,$(SRC_CUDA))


# Build rules
all: $(TARGET_SERIAL) $(TARGET_PTHREAD) $(TARGET_OPENMP) $(TARGET_CUDA)

#
$(TARGET_SERIAL): $(OBJ_SERIAL)
	$(CC) $^ -o $@
	CFLAGS2=

$(TARGET_PTHREAD): $(OBJ_PTHERAD)
	$(CC) $^ -o $@
	CFLAGS2=-pthread

$(TARGET_OPENMP): $(OBJ_OPENMP)
	$(CC) $^ -o $@
	CFLAGS2=-fopenmp

$(TARGET_CUDA): $(OBJ_CUDA)
	$(CC) $^ -o $@
	CFLAGS2=


%.o: %.cpp
	$(CC) $(CFLAGS) $(CFLAGS2) $< -o $@

# Phony targets
.PHONY: clean all

# Clean rule
clean:
	rm -f *.o $(TARGET_SERIAL) $(TARGET_PTHREAD) $(TARGET_OPENMP) $(TARGET_CUDA)
