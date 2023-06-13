#ifndef TYPE
#define TYPE

typedef enum{STOP, MOVE, OPEN} ElState;
typedef enum{WAIT, INCAR, ARRIVED} PeState;
typedef enum{NO, UP, DOWN} Direction;

typedef struct{
    ElState state;
    Direction dir;
    unsigned short loc;
    unsigned short des;
    unsigned short id;
}ElevatorState;

typedef struct{
    unsigned short des;
    unsigned short first;
    PeState state;
    Direction dir;
}Demand;

typedef struct{
    unsigned int I;
    unsigned int I2O;
    unsigned int O2I;
}bitmap;

typedef struct
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
}SharedData;

#endif