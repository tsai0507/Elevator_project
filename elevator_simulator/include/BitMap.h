#ifndef BITMAP
#define BITMAP
#include <stdlib.h>

void find_bitmap_nearest_0_index(unsigned int bitmap, int *nearest_index);
void find_all_bitmap_1_index(unsigned int bitmap, int *one_count, int *one_index);
void set_bitmap_1(unsigned int *bitmap, int bitmap_index);
void set_bitmap_0(unsigned int *bitmap, int bitmap_index);

#endif