# Compiler and compiler flags
CC=g++
CFLAGS=-c -Wall -O3
CFLAGS2=

# Source files
SRCS=$(filter-out image_adjustment%.cpp, $(wildcard *.cpp))
# $(info test, SRCS= $(SRCS))
# Object files
#OBJS=$(patsubst %.cpp,%.o,$(SRCS))

# Target executable (serial)
TARGET_SERIAL=image_adjustment
SRC_SERIAL=image_adjustment.cpp $(SRCS)
OBJ_SERIAL=$(patsubst %.cpp,%.o,$(SRC_SERIAL))
TMP_TARGET_SERIAL=serial

# Target executable (pthread)
TARGET_PTHREAD=image_adjustment_pthread
SRC_PTHERAD=image_adjustment_pthread.cpp $(SRCS)
OBJ_PTHERAD=$(patsubst %.cpp,%.o,$(SRC_PTHERAD))
TMP_TARGET_PTHREAD=pthread

# Target executable (openmp)
TARGET_OPENMP=image_adjustment_openmp
SRC_OPENMP=image_adjustment_openmp.cpp $(SRCS)
OBJ_OPENMP=$(patsubst %.cpp,%.o,$(SRC_OPENMP))
TMP_TARGET_OPENMP=openmp

# Target executable (cuda)
TARGET_CUDA=image_adjustment_cuda
SRC_CUDA=image_adjustment_cuda.cpp $(SRCS)
OBJ_CUDA=$(patsubst %.cpp,%.o,$(SRC_CUDA))
TMP_TARGET_CUDA=cuda

# Build rules
all: $(TMP_TARGET_SERIAL) $(TMP_TARGET_PTHREAD) $(TMP_TARGET_OPENMP) $(TMP_TARGET_CUDA)

#
$(TARGET_SERIAL): $(OBJ_SERIAL)
	$(CC) $^ -o $@
	CFLAGS2=

$(TARGET_PTHREAD): $(OBJ_PTHERAD)
	$(CC) $^ -pthread -o $@

$(TARGET_OPENMP): $(OBJ_OPENMP)
	$(CC) -fopenmp $^ -o $@

$(TARGET_CUDA): $(OBJ_CUDA)
	$(CC) $^ -o $@

$(TMP_TARGET_SERIAL):
	$(MAKE) $(TARGET_SERIAL)

$(TMP_TARGET_PTHREAD):
	$(MAKE) CFLAGS2=-pthread $(TARGET_PTHREAD)

$(TMP_TARGET_OPENMP):
	$(MAKE) CFLAGS2=-fopenmp $(TARGET_OPENMP)

$(TMP_TARGET_CUDA):
	$(MAKE) CFLAGS2= $(TARGET_CUDA)

%.o: %.cpp
	$(CC) $(CFLAGS) $(CFLAGS2) $< -o $@

# Phony targets
.PHONY: clean all

# Clean rule
clean:
	rm -f *.o $(TARGET_SERIAL) $(TARGET_PTHREAD) $(TARGET_OPENMP) $(TARGET_CUDA)
