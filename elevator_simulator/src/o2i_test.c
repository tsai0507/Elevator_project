#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include "type.h"

int shmid[32], Bitmap_id, shm_fd;
Demand *shm_data[32];
bitmap *shm_bitmap;


typedef struct
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} share_mut_cond;

share_mut_cond *shm_mut_cond;

void signal_handler(int signum)
{
    int i;
    for (i=0; i<32; i++)
    {
        shmdt(shm_data[i]);
    }
    shmdt(shm_bitmap);
    munmap(shm_mut_cond, sizeof(share_mut_cond));
    close(shm_fd);
    exit(signum);
}

void handler(int signum)
{
    //find all bitmap 1 function
    //
}

int main()
{
    signal(SIGINT, signal_handler);
    /* catch IO signal construct */
    struct sigaction my_action;
    memset(&my_action, 0, sizeof(struct sigaction));
    my_action.sa_flags = SA_SIGINFO;
    my_action.sa_sigaction = handler;
    sigaction(SIGUSR1, & my_action, NULL);
    shm_fd = shm_open("/sharememory", O_RDWR, 0666);
    shm_mut_cond = mmap(NULL, sizeof(share_mut_cond), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    int i;
    key_t key_data[32], key_bitmap = 1111;
    for (i=0; i<32; i++)
    {
        key_data[i] = i + 1;
        shmid[i] = shmget(key_data[i], sizeof(Demand), 0666);
        shm_data[i] = shmat(shmid[i], NULL, 0);
    }
    Bitmap_id = shmget(key_bitmap, sizeof(bitmap), 0666);
    shm_bitmap = (bitmap *)shmat(Bitmap_id, NULL, 0);
    i = 0;
    while (shm_bitmap->I_Checkbitmap > 0)
    {
        sleep(2);
        pthread_mutex_lock(&shm_mut_cond->mutex);
        shm_bitmap->O2I_Checkbitmap |= 1 << i;
        pthread_mutex_unlock(&shm_mut_cond->mutex);
        pthread_cond_signal(&shm_mut_cond->cond);
        i++;
    }
    return 0;
}