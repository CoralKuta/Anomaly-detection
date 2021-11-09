/*
 * anomaly_detection_util.cpp
 *
 * Author: Noa Eitan 316222777, Coral Kuta 208649186
 */

#include "SimpleAnomalyDetector.h"

#include <utility>

const float MULTIPLY_DEV = 1.1;

/**
 * function: SimpleAnomalyDetector
 * constructor.
 */
SimpleAnomalyDetector::SimpleAnomalyDetector() {
}

/**
 * function: SimpleAnomalyDetector
 * destructor.
 */
SimpleAnomalyDetector::~SimpleAnomalyDetector() {
}

/**
 * function: learnNormal.
 * @param ts a TimeSeries object.
 * The function learn a normal data from ts and add correlation features to cf member.
 */
void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts){
    // get vector of vectors, of floats, each init vector is feature
    const vector<vector<float>>features = ts.getFeatures();
    // scan every init vector (feature)
    for (int i = 0; i < features.size() ; i ++) {
        // initial
        float maxCorrelation = 0;
        int matchingColumn = -1;
        int j = i + 1;
        int size = (int) features[i].size();
        float feature1[size];
        for (int k = 0; k < size; k++) {
            feature1[k] = features[i][k];
        }
        float feature2[size];
        // scan the next features
        for (; j < features.size(); j++) {
            // scan the feature and add to feature1, feature2 array
            for (int k = 0; k < size; k++) {
                feature2[k] = features[j][k];
            }
            // calculate pearson
            float p = pearson(feature1, feature2, size);
            // save p as non negative
            if (p < 0) {
                p = p * (-1);
            }
            //if the absolute value of pearson is greater than maxCorrelation , save the details
            if (p > maxCorrelation) {
                maxCorrelation = p;
                matchingColumn = j;
            }
        }
        // if there is matching columns - add correlation to vector of correlations
        if (matchingColumn != (-1)) {
            for (int k = 0; k < size; k++) {
                feature2[k] = features[matchingColumn][k];
            }
            addCorrelation(ts.getFeatureName(i), ts.getFeatureName(matchingColumn),
                           feature1, feature2, size, maxCorrelation);
        }
    }
}

/**
 * function: detect.
 * @param ts a TimeSeries object.
 * The function scan data from ts and add report deviations, by the correlations from learnNormal.
 */
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts) {
    // initial vector of reports
    vector<AnomalyReport> reports;
    // save the data from ts in lines, by samples
    vector<vector<float>> lines = ts.getSamples();
    vector<correlatedFeatures> normalData = this->getNormalModel();
    // scan every init vector
    for (int row = 0; row < lines.size(); row ++) {
        // scan every correlated features
        for (int j = 0; j < normalData.size(); j ++) {
            // save the indexes of the features
            int indexFeature1 = ts.getFeatureCol(normalData[j].feature1);
            int indexFeature2 = ts.getFeatureCol(normalData[j].feature2);
            // initial point with the data from the two features, in this row
            Point checkPoint = Point(lines[row][indexFeature1], lines[row][indexFeature2]);
            // calculate the deviation between the point and the linear regression of the correlation features
            float deviation = dev(checkPoint, normalData[j].lin_reg);
            // if the deviation is greater than the threshold - there is a deviation! report
            if (deviation > normalData[j].threshold) {
                //create object of AnomalyReport and push into the vector of reports
                string description = normalData[j].feature1 + "-" + normalData[j].feature2;
                long timeStep = row + 1;
                AnomalyReport newReport = AnomalyReport(description, timeStep);
                reports.push_back(newReport);
            }
        }
    }
    return reports;
}

/**
* function: addCorrelation.
* @param feature1 the name of the first feature in the correlation
* @param feature2 the name of the second feature in the correlation
* @param x the data in feature1
* @param y the data in feature2
* @param size size of x,y
* @param correlation the correlation between the features
* The function add correlation into the cf vector of correlations.
*/
void SimpleAnomalyDetector::addCorrelation(string feature1, string feature2, float *x, float *y, int size,
                                           float correlation) {
    // create new array of points.
    Point* points[size];
    float maxDev = 0;
    //create array of points, with the values of x, y
    for (int i = 0; i < size; i ++) {
        points[i] = new Point(x[i], y[i]);
    }
    // calculate the linear regression of these features, by points of the features (feature1, feature2)
    Line linearReg = linear_reg(points, size);
    // scan the points and calculate the deviation of each point, and save the maximum deviation
    for (int i = 0; i < size; i ++ ) {
        float deviation = dev(Point(x[i], y[i]), linearReg);
        if (deviation > maxDev) {
            maxDev = deviation;
        }
    }
    // free allocations
    for (int i = 0; i < size; i ++) {
        delete points[i];
    }
    // if the correlation greater than the threshold, we add the correlation to the vector of correlations.
    if (correlation >= THRESHOLD) {
        // create new object of correlatedFeatures
        correlatedFeatures newCorrelation;
        newCorrelation.feature1 = feature1;
        newCorrelation.feature2 = feature2;
        newCorrelation.corrlation = correlation;
        newCorrelation.lin_reg = linearReg;
        newCorrelation.threshold = maxDev * MULTIPLY_DEV;
        //insert into cf (vector of correlatedFeatures) the new correlation
        cf.push_back(newCorrelation);
    }
}