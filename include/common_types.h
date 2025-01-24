#pragma once // #include one time at most

#include <stdbool.h> 

// Pointer is  more readable from void *
typedef void* Pointer;

typedef unsigned int uint; // If i need it

// Pointer to function that compares 2 elements and returns:
// < 0  if a < b
//   0  if a and b are equal
// > 0  if a > b
typedef int (*CompareFunc)(Pointer a, Pointer b);

// Pointer to function that destroyes an element value
typedef void (*DestroyFunc)(Pointer value);