#include <stdlib.h>
#include "func.h"

int *create_int(int i){
    int* pointer = malloc(sizeof(int));
    *pointer = i;
    return pointer;
}

Details *create_details(Details det) {
    Details* pointer = malloc(sizeof(Details));
    *pointer = det;
    return pointer;
}

int compare_ints(Pointer a, Pointer b) {
	return *(int*)a - *(int*)b;
}

int compare_details(Pointer a, Pointer b) {
	return ((Details*)a)->prime - ((Details*)b)->prime;
}

void generate_ranges(int min, int max, int number_of_ranges, struct Range ranges[]) {
    int i;
    int bucket_sizes[number_of_ranges];

    int even_length = (max-min+1)/number_of_ranges;
    for(i=0; i<number_of_ranges; i++)
        bucket_sizes[i] = even_length;

    /* distribute surplus as evenly as possible across buckets */
    int surplus = (max-min+1)%number_of_ranges;
    for(i=0; surplus>0; --surplus, i=(i+1)%number_of_ranges)
        bucket_sizes[i] += 1; 

    int n=0, k=min;
    for(i=0; i<number_of_ranges && k<=max; ++i, ++n){
        ranges[i].lo=k;
        ranges[i].hi=k+bucket_sizes[i]-1;
        k += bucket_sizes[i];
    }
}