#include "BitMap.h"

void find_bitmap_nearest_0_index(unsigned int bitmap, int *nearest_index)
{
    *nearest_index = 0;
    while (((~bitmap & 0xffffffff) & 1 << *nearest_index) == 0)
    {
        *nearest_index += 1;
    }
}

void find_all_bitmap_1_index(unsigned int bitmap, int *one_count, int *one_index)
{
    int i = 0;
    *one_count = 0;
    while (i < 32)
    {
        if (bitmap & (1 << i))
        {
            one_index[*one_count] = i;
            *one_count += 1;
        }
        i++;
    }
}

void set_bitmap_1(unsigned int *bitmap, int bitmap_index)
{
    *bitmap |= 1 << bitmap_index;
}

void set_bitmap_0(unsigned int *bitmap, int bitmap_index)
{
    *bitmap ^= 1 << bitmap_index;
}