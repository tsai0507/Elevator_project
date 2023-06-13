#include "elevator_tool.h"

void *elevator(void *parm){
    
    ElevatorState *Ele = (ElevatorState*)parm;
    short ID = Ele->id;
    short TimFlag;
    bool passage[32] = {0}, getPeoF, putPeoF;
    struct timespec req, rem;
    memset(&req, 0, sizeof(struct timespec));
    req.tv_nsec = 0;

    int shm_fd;
    SharedData *shm_mut_cond;
    shm_mut_cond = create_mutex_lock(shm_fd);
    
    while(1){
        if(Ele->state != STOP){  
            getPeoF = get_people(Ele, passage);
            putPeoF = putdown_people(Ele, passage);
            if(getPeoF || putPeoF){
                Ele->state = OPEN;
                printf("[STATE] Elevator %d: Open at %d\n", ID, Ele->loc);
                if(getPeoF)
                    printf("[SERVE] Elevator %d: Get people\n", Ele->id);
                if(putPeoF)
                    printf("[SERVE] Elevator %d: Put people\n", Ele->id);
                printf("-----------------------------------------\n");
                /* Refresh data of some body get out the car and wake up IO */
                pthread_mutex_lock(&shm_mut_cond->mutex);
                pthread_mutex_unlock(&shm_mut_cond->mutex);
                pthread_cond_signal(&shm_mut_cond->cond);

                req.tv_sec = 2;
                TimFlag = nanosleep(&req, &rem); 
                if (TimFlag == -1)
                    nanosleep(&rem, &rem);

                /* If need stop */
                if(Ele->loc == Ele->des){                   
                    printf("[STATE] Elevator %d: Stop at %d\n", ID, Ele->loc);
                    printf("-----------------------------------------\n");
                    Ele->state = STOP;
                    Ele->dir = NO;
                }   
                else{
                    Ele->state = MOVE;
                }       
            }
        }
        if(Ele->state == MOVE){
            if(Ele->dir == UP) 
                Ele->loc += 1;
            else if(Ele->dir == DOWN)
                Ele->loc -= 1;
            req.tv_sec = 3;
            TimFlag = nanosleep(&req, &rem); 
            if (TimFlag == -1)
                nanosleep(&rem, &rem); 
            if(Ele->loc == Ele->des){
                Ele->dir = NO;
            }  
        }
    }
}

bool ifLonger(unsigned short des, ElevatorState ele){
    int cost = des - ele.des;
    if(ele.dir == UP && cost > 0) return true;
    else if (ele.dir == DOWN && cost < 0) return true;
    else if (ele.dir == NO) return false;
    else return false;
}

bool AllocLongerDest(Demand* peo, ElevatorState **ele){
    bool flag1, flag2;
    int dis1, dis2, dis;
    flag1 = ifLonger(peo->first, *ele[0]);
    flag2 = ifLonger(peo->first, *ele[1]);
    dis1 = DistanceCost(peo->first, peo->dir, *ele[0]);
    dis2 = DistanceCost(peo->first, peo->dir, *ele[1]);
    dis = -1;
    if(flag1 && flag2){
        if(dis1 <= dis2 && ele[0]->state != STOP && ele[0]->dir != NO)
        {
            ele[0]->des = peo->first; 
            dis = dis1;
            printf("[INFO] Elevator 1 allocte longer destination to %d\n", peo->first);
            printf("-----------------------------------------\n");
        } 
        else if(dis1 > dis2 && ele[1]->state != STOP && ele[1]->dir != NO)
        {
            ele[1]->des = peo->first; 
            dis = dis2;
            printf("[INFO] Elevator 2 allocte longer destination to %d\n", peo->first);
            printf("-----------------------------------------\n");
        } 
    }
    else if(flag1 == true && flag2 == false){
        if(dis1 < dis2 && ele[0]->state != STOP && ele[0]->dir != NO)
        {
            ele[0]->des = peo->first; 
            dis = dis1;
            printf("[INFO] Elevator 1 allocte longer destination to %d\n", peo->first);
            printf("-----------------------------------------\n");
        } 
    }
    else if(flag1 == false && flag2 == true){
        if(dis2 < dis1 && ele[1]->state != STOP && ele[1]->dir != NO)
        {
            ele[1]->des = peo->first;  
            dis = dis2;
            printf("[INFO] Elevator 2 allocte longer destination to %d\n", peo->first);
            printf("-----------------------------------------\n");
        } 
    }

    if(dis == 0) return true;
    else return false;
}

int DistanceCost(unsigned short des, Direction dir, ElevatorState ele){
    int lenCD, lenCP;
    lenCD = ele.des - ele.loc;
    lenCP = des - ele.loc;
    if(lenCP == 0) return 0;
    else if(ele.dir == UP){
        if(dir == UP && lenCP > 0) return lenCP;
        else if(dir != UP && lenCP > 0) 
            return (abs(des - ele.des) + lenCD); 
        else{
            lenCP = abs(des - ele.des) + lenCD;
            return lenCP;
        }
    }
    else if(ele.dir == DOWN){
        if(dir == DOWN && lenCP < 0) return abs(lenCP);
        else if(dir != DOWN && lenCP < 0) {
            return (abs(des - ele.des) + abs(lenCD));
        }
        else{
            lenCP = abs(des - ele.des) + abs(lenCD);
            return lenCP;
        }
    }
    else return abs(lenCP);
}

unsigned short ForSTOPchoseDest(ElevatorState *main, ElevatorState *another){
    int moveDis = 0, comDis = 0, anotherDis;
    unsigned short dest = 0;
    int index, one_count, one_index[32];

    find_all_bitmap_1_index(shm_bitmap->I, &one_count, one_index);
    for(int i = 0; i < one_count; i++){
        index = one_index[i];
        if(p_Demand[index]->state == WAIT){
            comDis = DistanceCost(p_Demand[index]->first, p_Demand[index]->dir, *main);
            /* For the request of same floor */
            if(comDis == 0 && 
                (another->des != p_Demand[index]->first || another->dir != p_Demand[index]->dir)){
                main->dir = p_Demand[index]->dir;
                return p_Demand[index]->first;
            }
            else if(comDis > moveDis && another->des != p_Demand[index]->first){
                anotherDis = DistanceCost(p_Demand[index]->first, p_Demand[index]->dir, *another);
                /* If another elevator is more quick */
                if((anotherDis > comDis) ||
                    (anotherDis == comDis && another->state != MOVE))
                {
                    dest = p_Demand[index]->first;
                    moveDis = comDis;
                }
            }
        }
    }
    return dest;
}

bool get_people(ElevatorState* ele, bool* get_passage){
    bool ret = false;
    int index, one_count, one_index[32];

    find_all_bitmap_1_index(shm_bitmap->I, &one_count, one_index);
    P(sem_ele);
    for(int i = 0; i < one_count; i++){
        index = one_index[i];
        if((p_Demand[index]->first==ele->loc) && (p_Demand[index]->state==WAIT))
        {
            if((ele->dir==NO) || (p_Demand[index]->dir==ele->dir)){
                if(ele->dir == NO)
                    ele->dir = p_Demand[index]->dir;
                p_Demand[index]->state = INCAR;
                get_passage[index] = 1;
                set_bitmap_1(&shm_bitmap->O2I, index);
                // printf("[SERVE] Elevator %d: Get people\n", ele->id);
                if(ifLonger(p_Demand[index]->des, *ele))
                    ele->des = p_Demand[index]->des;
                ret = true;
            }
        }
    }
    V(sem_ele);
    return ret;
}

bool putdown_people(ElevatorState* ele, bool *passage){
    bool ret = false;
    int index, one_count, one_index[32];

    find_all_bitmap_1_index(shm_bitmap->I, &one_count, one_index);
    P(sem_ele);
    for(int i = 0; i < one_count; i++){
        index = one_index[i];
        if(passage[index] && p_Demand[index]->des == ele->loc){
            p_Demand[index]->state = ARRIVED;
            passage[index] = 0;
            // set_bitmap_0(&shm_bitmap->I, index);
            set_bitmap_1(&shm_bitmap->O2I, index);
            // printf("[SERVE] Elevator %d: Put people\n", ele->id);
            ret = true;
        }
    }
    V(sem_ele);
    return ret;
}