/*
 * anomaly_detection_util.cpp
 *
 * Author:
 */

#include <cmath>
#include "anomaly_detection_util.h"


float avg(float* x, int size){
	return 0;
}

// returns the variance of X
float var(float* x, int size){
    // making sure the size if the array is legit
    if (size < 1) {
        return 0;
    }

    float sum = 0;

    // summing the squared values of the x array
    for (int i = 0; i < size; i++) {
        sum += (x[i] * x[i]);
    }
    sum = sum / float (size);

    //calculate the average of the original x array
    float average = avg(x, size);
    return sum - (average * average);
}

// a function that receives an array and returns a new array with each value minus it's average.
float* minus_avg(float* arr, int size) {
    auto *new_arr = new float[size];
    float average = avg(arr, size);

    //subtract the average from each value of the array
    for (int i = 0; i < size; i++) {
        new_arr[i] = arr[i] - average;
    }
    return new_arr;
}

// a function that receives 2 arrays and return one array with the values multiplied.
float* mult_arrays(const float* x, const float* y, int size) {
    auto *new_arr = new float[size];

    //multiplying x[i] with y[i] for each i.
    for (int i = 0; i < size; i++) {
        new_arr[i] = x[i] * y[i];
    }
    return new_arr;
}

// returns the covariance of X and Y
float cov(float* x, float* y, int size){
    // making sure the size if the array is legit
    if (size < 1) {
        return 0;
    }

    //getting the decreased arrays of x and y
    float* new_x = minus_avg(x, size);
    float* new_y = minus_avg(y, size);

    //returning the average of the multiplied array
    return avg(mult_arrays(new_x, new_y, size), size);
}


// returns the Pearson correlation coefficient of X and Y
float pearson(float* x, float* y, int size) {
    // using the "sqrtf" function to calculate the square root of a float.
    float standard_deviation_x = sqrtf(var(x, size));
    float standard_deviation_y = sqrtf(var(y, size));

    // making sure to not divide by 0
    if (standard_deviation_x == 0 || standard_deviation_y == 0) {
        return 0;
    }

	return (cov(x, y, size)) / (standard_deviation_x * standard_deviation_y);
}

// performs a linear regression and returns the line equation
Line linear_reg(Point** points, int size){

	return Line(0,0);
}

// returns the deviation between point p and the line equation of the points
float dev(Point p,Point** points, int size){
	return 0;
}

// returns the deviation between point p and the line
float dev(Point p,Line l){
	return 0;
}




