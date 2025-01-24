#include "common_types.h"

#pragma once // #include το πολύ μία φορά

typedef struct Range{
    int lo;
    int hi;
}Range;

typedef struct details{
    int prime;
    double time;
}Details;

int *create_int(int i);

Details *create_details(Details det);

int compare_ints(Pointer a, Pointer b);

int compare_details(Pointer a, Pointer b);

void generate_ranges(int min, int max, int number_of_ranges, struct Range ranges[]);