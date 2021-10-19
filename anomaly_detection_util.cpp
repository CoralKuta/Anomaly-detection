/*
 * anomaly_detection_util.cpp
 *
 * Author: Noa Eitan 316222777, Coral Kuta 208649186
 */

#include <cmath>
#include "anomaly_detection_util.h"
#include <exception>

float* get_X_from_points(Point** points, int size);
float* get_Y_from_points(Point** points, int size);

// return the average of the values that in the array
float avg(const float* x, int size){
    // size is 0 or less. return 0.
    if (size < 1) {
        return 0;
    }
    float sum = 0;

    // sum the values from the array
    for (int i = 0; i < size; i++) {
        sum += x[i];
    }

    // return the average
    return (sum / (float) size);
}

// returns the variance of X and Y
float var(float* x, int size){
	return size;
}

// returns the covariance of X and Y
float cov(float* x, float* y, int size){
	return 0;
}


// returns the Pearson correlation coefficient of X and Y
float pearson(float* x, float* y, int size){
	return 0;
}

// performs a linear regression and returns the line equation
Line linear_reg(Point** points, int size){
    float a, b;

    // initial arrays of x and y values, of the points
    float *x = get_X_from_points(points, size);
    float *y = get_Y_from_points(points, size);

    // calculate a
    a = cov(x, y, size) / var(x, size);

    // calculate b
    float average_x = avg(x, size);
    float average_y = avg(y, size);
    b = average_y - (a * average_x);
	return {a, b};
}

// return array of x values of points, from array of points
float* get_X_from_points(Point** points, int size){
    auto* arr = new float[size];

    // scan the array of points and initial new array with the x values of the points
    for (int i = 0; i < size; i++) {
        arr[i] = points[i]->get_x();
    }
    return arr;
}

// return array of y values of points, from array of points
float* get_Y_from_points(Point** points, int size){
    auto* arr = new float[size];

    // scan the array of points and initial new array with the x values of the points
    for (int i = 0; i < size; i++) {
        arr[i] = points[i]->get_y();
    }
    return arr;
}

// returns the deviation between point p and the line equation of the points
float dev(Point p,Point** points, int size){
    // save the line from the array of points
	Line line = linear_reg(points, size);

    // return the deviation between point p and the line
    return dev(p, line);
}

// returns the deviation between point p and the line
float dev(Point p,Line l){
    // save the y value of the point
    float y = p.get_y();

    // get the real value of y (f(x))
    float fx = l.f(p.get_x());

    // return the absolute distance between the y value of the point and the y value of the line
    return fabsf(y - fx);
}




