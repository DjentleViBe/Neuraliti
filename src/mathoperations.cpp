#include "stdc++.h"
using namespace std;

float* concatenate(float arr1[], float arr2[], int m, int n, size_t arra2)
{

    memcpy(arr1 + m, arr2, arra2);
    return arr1;
}

unsigned int* concatenate(unsigned int arr1[], unsigned int arr2[], int m, int n, size_t arra2)
{
    memcpy(arr1 + m, arr2, arra2);
    return arr1;
}
