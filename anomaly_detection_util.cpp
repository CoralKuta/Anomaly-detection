/*
 * anomaly_detection_util.cpp
 *
 * Author: Noa Eitan 316222777, Coral Kuta 208649186
 */

#include <cmath>
#include "anomaly_detection_util.h"


// return the average of the values that in the array
float avg(float* x, int size){
    float sum = 0;

    // sum the values from the array
    for (int i = 0; i < size; i++) {
        sum += x[i];
    }

    // return the average
    return (sum / (float) size);
}

// returns the variance of X
float var(float* x, int size){
    float sum = 0;

    // summing the squared values of the x array
    for (int i = 0; i < size; i++) {
        sum += (x[i] * x[i]);
    }
    sum = sum / (float) size;

    //calculate the average of the original x array
    float average = avg(x, size);
    return sum - (average * average);
}


// returns the covariance of X and Y
float cov(float* x, float* y, int size){
    // calculating the average of x and y
    float average_x = avg(x, size);
    float average_y = avg(y, size);

    // creating new arrays to store the values minus their average
    float *x_minus = new float [size];
    float *y_minus = new float [size];

    for (int i = 0; i < size; i++) {
        x_minus[i] = (x[i] - average_x);
        y_minus[i] = (y[i] - average_y);
    }

    float *multiplied = new float [size];

    for (int i = 0; i < size; i++) {
        multiplied[i] = (x_minus[i] * y_minus[i]);
    }

    float average = avg(multiplied, size);

    // delete the arrays
    delete[] x_minus;
    delete[] y_minus;
    delete[] multiplied;

    return average;
}


// returns the Pearson correlation coefficient of X and Y
float pearson(float* x, float* y, int size) {
    // calculate the square root of the variance.
    float standard_deviation_x = sqrtf(var(x, size));
    float standard_deviation_y = sqrtf(var(y, size));

	return (cov(x, y, size)) / (standard_deviation_x * standard_deviation_y);
}

// performs a linear regression and returns the line equation
Line linear_reg(Point** points, int size){
    float a, b;

    // initial arrays of x and y values, of the points
    float *x = new float [size];
    float *y = new float [size];

    // going through the points array and creating x values array and y values array
    for (int i = 0; i < size; i++) {
        x[i] = points[i]->x;
        y[i] = points[i]->y;
    }

    // calculate a
    a = cov(x, y, size) / var(x, size);

    // calculate b
    float average_x = avg(x, size);
    float average_y = avg(y, size);
    b = average_y - (a * average_x);

    // deleting the new objects
    delete[] x;
    delete[] y;

	return {a, b};
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
    float y = p.y;

    // get the real value of y (f(x))
    float fx = l.f(p.x);

    // return the absolute distance between the y value of the point and the y value of the line
    if (y >= fx) {
        return (y - fx);
    } else {
        return (fx - y);
    }
}




