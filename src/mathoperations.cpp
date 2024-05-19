#include "../dependencies/include/stdc++.h"
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

float calculate_distance(float x1, float y1, float x2, float y2){
    float distance = sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));
    // std::cout << distance << std::endl;
    return distance;
}

// Function to calculate the dot product of two vectors
double dotProduct(double x1, double y1, double x2, double y2) {
    return x1 * x2 + y1 * y2;
}

// Function to calculate the magnitude of a vector
double magnitude(double x, double y) {
    return sqrt(x * x + y * y);
}

// Function to calculate the angle (in radians) between two points
double calculate_angle(double x1, double y1, double x2, double y2) {
    // Calculate the dot product of the two vectors
    // double y = y2 - y1;
    double x = x2 - x1;
    double h = calculate_distance(x1, y1, x2, y2);
    
    // Convert the angle to degrees if needed
    // double angleDeg = angleRad * 180.0 / M_PI;
    double angleRad = acos(x / h);
    
    return angleRad;
}

