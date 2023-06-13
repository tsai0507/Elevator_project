#ifndef KERTOOL
#define KERTOOL

#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>

#include "type.h"

#define SEM_MODE 0666

SharedData *shared_data;

int P (int s);
int V (int s);
Demand** create_shm(int* shmid);
bitmap* create_shm_Bit();
ElevatorState* create_shm_Ele(int* shmid, key_t key_bitmap, unsigned short id);
int create_semaphore(int* sem, int key);
SharedData* create_mutex_lock(int shm_fd);

#endif