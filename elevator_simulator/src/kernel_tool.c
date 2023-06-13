#include "kernel_tool.h"

/* acquire */
int P (int s)
{
    struct sembuf sop;
    sop.sem_num = 0;
    sop.sem_op = -1;
    sop.sem_flg = 0;
}

/* release */
int V(int s)
{
    struct sembuf sop;
    sop.sem_num = 0;
    sop.sem_op = 1;
    sop.sem_flg = 0;
}

int create_semaphore(int* sem, int key)
{
    *sem = semget(key, 1, IPC_CREAT | SEM_MODE);
    semctl(*sem, 0, SETVAL, 1);
}

SharedData* create_mutex_lock(int shm_fd)
{
    shm_fd = shm_open("/sharememory", O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(SharedData));
    shared_data = mmap(NULL, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_init(&mutex_attr);
    pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&shared_data->mutex, &mutex_attr);
    
    pthread_condattr_t cond_attr;
    pthread_condattr_init(&cond_attr);
    pthread_condattr_setpshared(&cond_attr, PTHREAD_PROCESS_SHARED);
    pthread_cond_init(&shared_data->cond, &cond_attr);

    return shared_data;
}

Demand** create_shm(int* shmid){

    int i;
    key_t key_data[32], key_bitmap = 1111;
    Demand **shm_data = (Demand**)malloc(sizeof(Demand*)*32);

    for (i=1; i <= 32; i++)
    {
        key_data[i-1] = i;
        shmid[i-1] = shmget(key_data[i-1], sizeof(Demand), IPC_CREAT | 0666);
        shm_data[i-1] = (Demand*)shmat(shmid[i-1], NULL, 0);
    }
    return shm_data;
}

bitmap* create_shm_Bit(int* shmid){

    bitmap* shm_bitmap;
    key_t key_bitmap = 1111;

    *shmid = shmget(key_bitmap, sizeof(bitmap), IPC_CREAT | 0666);
    shm_bitmap = (bitmap *)shmat(*shmid, NULL, 0);
    /* set bitmap initial value */
    shm_bitmap->I2O = 0;
    shm_bitmap->I = 0;
    shm_bitmap->O2I = 0;

    return shm_bitmap;
}

ElevatorState* create_shm_Ele(int* shmid, key_t key_bitmap, unsigned short id){
    ElevatorState* shm_ele;
    *shmid = shmget(key_bitmap, sizeof(ElevatorState), IPC_CREAT | 0666);
    shm_ele = (ElevatorState *)shmat(*shmid, NULL, 0);
    shm_ele->state = STOP;
    shm_ele->dir = NO;
    shm_ele->loc = 5;
    shm_ele->des = 5;
    shm_ele->id = id;

    return shm_ele;
}
