# CC = gcc
CC = /usr/bin/aarch64-linux-gnu-gcc

SRC = $(wildcard src/)
OBJ = obj
INCLUDE = include
OBJS = $(OBJ)/BitMap.o $(OBJ)/elevator_tool.o $(OBJ)/device_output.o $(OBJ)/kernel_tool.o

all: system_test IO

system_test: ${SRC}system_test.c $(OBJS)
	$(CC) -I$(INCLUDE) -o system_test $^ -lpthread  -lrt

IO: ${SRC}IO.c
	$(CC) -I$(INCLUDE) -o IO ${SRC}IO.c -lrt -lpthread

$(OBJ)/BitMap.o: ${SRC}BitMap.c
	$(CC) -I$(INCLUDE) -c $^ -o $@

$(OBJ)/elevator_tool.o: ${SRC}elevator_tool.c
	$(CC) -I$(INCLUDE) -c $^ -o $@

$(OBJ)/device_output.o: ${SRC}device_output.c  
	$(CC) -I$(INCLUDE) -c $^ -o $@

$(OBJ)/kernel_tool.o: ${SRC}kernel_tool.c
	$(CC) -I$(INCLUDE) -c $^ -o $@

clean: 
	rm $(OBJ)/*.o system_test IO