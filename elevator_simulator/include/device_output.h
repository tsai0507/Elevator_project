#ifndef DEVICE
#define DEVICE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "kernel_tool.h"

#define SEM_KEY_DEVICE 0001

int sem_device;

void elevator1_device_output();
void elevator2_device_output();

#endif