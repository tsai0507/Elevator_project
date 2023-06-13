#include "device_output.h"

void elevator1_device_output(ElevatorState* Ele)
{
    /* for blocking, can be marked */
    // while(1){
    //     usleep(1000000);
    // }

    /* file device */
    int fd_led;
    fd_led = open("/dev/led_device", O_WRONLY);
    if (fd_led < 0) {
        printf("Failed to open led device\n");
    }

    /* initialize write device array */
    char dev_state[13];
    strcpy(dev_state, "110000000000"); // default 7 seg all on

    int mode, floor;
    while(1){

        /* read state */
        mode = Ele->state;
        floor = Ele->loc;

        /* update */
        dev_state[1] = '1';
        dev_state[2] = floor + '0'; 
        if(mode == 0){ // moving
            dev_state[4] = '1'; // red light
            dev_state[3] = '0';
            dev_state[5] = '0';
            
            P(sem_device);
            write(fd_led, dev_state, sizeof(dev_state));
            V(sem_device);
            
            usleep(0.1 * 1000000);
        }
        else if(mode == 1){ // no use
            dev_state[4] = '0';
            dev_state[3] = '1'; // green light
            dev_state[5] = '0';

            P(sem_device);
            write(fd_led, dev_state, sizeof(dev_state));
            V(sem_device);

            usleep(0.1 * 1000000);
        }
        else if(mode == 2){ // open and closing
            dev_state[4] = '0';
            dev_state[3] = '0';
            dev_state[5] = '1'; // orange light

            P(sem_device);
            write(fd_led, dev_state, sizeof(dev_state));
            V(sem_device);
            usleep(0.5 * 1000000);
            dev_state[1] = '0';
            P(sem_device);
            write(fd_led, dev_state, sizeof(dev_state));
            V(sem_device);
            usleep(0.5 * 1000000);
        }
        
    }
    
}

void elevator2_device_output(ElevatorState* Ele)
{
    /* for blocking, can be marked */
    // while(1){
    //     usleep(1000000);
    // }
    
    /* file device */
    int fd_led;
    fd_led = open("/dev/led_device", O_WRONLY);
    if (fd_led < 0) {
        printf("Failed to open led device\n");
    }

    /* initialize write device array */
    char dev_state[13];
    strcpy(dev_state, "000000110000"); // default 7 seg all on

    int mode, floor;
    while(1){

        /* read state */
        mode = Ele->state;
        floor = Ele->loc;

        /* update */
        dev_state[7] = '1';
        dev_state[8] = floor + '0'; 
        if(mode == 0){ // moving
            dev_state[10] = '1'; // red light
            dev_state[9] = '0';
            dev_state[11] = '0';
            
            P(sem_device);
            write(fd_led, dev_state, sizeof(dev_state));
            V(sem_device);

            usleep(0.1 * 1000000);
        }
        else if(mode == 1){ // no use
            dev_state[10] = '0';
            dev_state[9] = '1'; // green light
            dev_state[11] = '0';

            P(sem_device);
            write(fd_led, dev_state, sizeof(dev_state));
            V(sem_device);

            usleep(0.1 * 1000000);
        }
        else if(mode == 2){ // open and closing
            dev_state[10] = '0';
            dev_state[9] = '0';
            dev_state[11] = '1'; // orange light

            P(sem_device);
            write(fd_led, dev_state, sizeof(dev_state));
            V(sem_device);
            usleep(0.5 * 1000000);
            dev_state[7] = '0';
            P(sem_device);
            write(fd_led, dev_state, sizeof(dev_state));
            V(sem_device);
            usleep(0.5 * 1000000);
        }
        
    }
}