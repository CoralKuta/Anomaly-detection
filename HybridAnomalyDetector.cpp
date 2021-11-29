/*
 * HybridAnomalyDetector.cpp
 *
 * Author: Noa Eitan 316222777, Coral Kuta 208649186
 */

#include "HybridAnomalyDetector.h"
float MULTIPLY_RADIUS = 1.1;

HybridAnomalyDetector::HybridAnomalyDetector() {
	// TODO Auto-generated constructor stub

}

HybridAnomalyDetector::~HybridAnomalyDetector() {
	// TODO Auto-generated destructor stub
}


/**
* function: addCorByMec.
* @param feature1 the name of the first feature in the correlation
* @param feature2 the name of the second feature in the correlation
* @param x the data in feature1
* @param y the data in feature2
* @param size size of x,y
* @param correlation the correlation between the features
* The function add correlation into the cf vector of correlations.
*/
void HybridAnomalyDetector::addCorByMec(string feature1, string feature2, float *x, float *y, int size,
                                        float correlation) {
    // create new array of points.
    Point *points[size];
    //create array of points, with the values of x, y
    for (int i = 0; i < size; i++) {
        points[i] = new Point(x[i], y[i]);
    }

    // finding the minimum enclosing circle of the given points.
    Circle mec = findMinCircle(points, size);

    // create new object of correlatedFeatures
    correlatedFeatures newCorrelation;
    newCorrelation.feature1 = feature1;
    newCorrelation.feature2 = feature2;
    newCorrelation.corrlation = correlation;
    newCorrelation.lin_reg = Line(0, 0);
    newCorrelation.threshold = (mec.radius) * MULTIPLY_RADIUS;
    newCorrelation.mec = mec;
    newCorrelation.isByReg = false;
    //insert into cf (vector of correlatedFeatures) the new correlation
    cf.push_back(newCorrelation);
    // free allocations
    for (int i = 0; i < size; i++) {
        delete points[i];
    }
}
