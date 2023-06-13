#ifndef ELEVATOR
#define ELEVATOR
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <pthread.h>
#include <sys/mman.h>
#include <fcntl.h>

#include "type.h"
#include "BitMap.h"
#include "kernel_tool.h"

#define SEM_KEY_ELE 2030

bool check_space[32];
bool NewRequest[32];
bool getNewRequest;
int sem_ele;

Demand **p_Demand;
bitmap *shm_bitmap;

void *elevator(void *parm);
unsigned short ForSTOPchoseDest(ElevatorState *main, ElevatorState *another); 
bool AllocLongerDest(Demand* peo, ElevatorState **ele);
bool ifLonger(unsigned short des, ElevatorState ele);
int DistanceCost(unsigned short des, Direction dir, ElevatorState ele);
bool get_people(ElevatorState* ele, bool* get_passage);
bool putdown_people(ElevatorState* ele, bool *passage);

#endif