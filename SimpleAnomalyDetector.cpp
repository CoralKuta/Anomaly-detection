/*
 * SimpleAnomalyDetector.cpp
 *
 * Author: Noa Eitan 316222777, Coral Kuta 208649186
 */

#include "SimpleAnomalyDetector.h"

const float MULTIPLY_DEV = 1.1;

/**
 * function: SimpleAnomalyDetector
 * constructor.
 */
SimpleAnomalyDetector::SimpleAnomalyDetector(float threshold): TimeSeriesAnomalyDetector() {
    this->threshold = threshold;
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
void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts) {
    // get vector of vectors, of floats, each init vector is feature
    const vector<vector<float>>features = ts.getFeatures();
    // scan every init vector (feature)
    for (int i = 0; i < features.size() ; i ++) {
        // initial
        float maxCorrelation = 0.5;
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

            if (maxCorrelation >= threshold) {
                addCorByReg(ts.getFeatureName(i), ts.getFeatureName(matchingColumn),
                            feature1, feature2, size, maxCorrelation);
            } else if ((maxCorrelation > 0.5) && (maxCorrelation < threshold)){
                addCorByMec(ts.getFeatureName(i), ts.getFeatureName(matchingColumn),
                            feature1, feature2, size, maxCorrelation);
            }
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
    // scan every correlated features
    for (int j = 0; j < normalData.size(); j ++) {
        // scan every init vector
        for (int row = 0; row < lines.size(); row ++) {
            // save the indexes of the features
            int indexFeature1 = ts.getFeatureCol(normalData[j].feature1);
            int indexFeature2 = ts.getFeatureCol(normalData[j].feature2);
            // initial point with the data from the two features, in this row
            Point checkPoint = Point(lines[row][indexFeature1], lines[row][indexFeature2]);
            // calculate the deviation between the point and the linear regression of the correlation features

            bool toReport = false;
            if (normalData[j].isByReg) {
                float deviation = dev(checkPoint, normalData[j].lin_reg);
                // if the deviation is greater than the threshold - there is a deviation! report
                if (deviation > normalData[j].threshold) {
                    toReport = true;
                }
            } else {
                Point center = normalData[j].mec.center;
                Circle tmpCircle = Circle(center, normalData[j].threshold);
                if (!is_inside(tmpCircle, checkPoint)) {
                    toReport = true;
                }
            }
            if (toReport) {
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
* function: addCorByReg.
* @param feature1 the name of the first feature in the correlation
* @param feature2 the name of the second feature in the correlation
* @param x the data in feature1
* @param y the data in feature2
* @param size size of x,y
* @param correlation the correlation between the features
* The function add correlation into the cf vector of correlations.
*/
void SimpleAnomalyDetector::addCorByReg(string feature1, string feature2, float *x, float *y, int size,
                                        float correlation) {
    // create new array of points.
    Point *points[size];
    float maxDev = 0;
    //create array of points, with the values of x, y
    for (int i = 0; i < size; i++) {
        points[i] = new Point(x[i], y[i]);
    }

    // calculate the linear regression of these features, by points of the features (feature1, feature2)
    Line linearReg = linear_reg(points, size);
    // scan the points and calculate the deviation of each point, and save the maximum deviation
    for (int i = 0; i < size; i++) {
        float deviation = dev(Point(x[i], y[i]), linearReg);
        if (deviation > maxDev) {
            maxDev = deviation;
        }
    }

    // create new object of correlatedFeatures
    correlatedFeatures newCorrelation;
    newCorrelation.feature1 = feature1;
    newCorrelation.feature2 = feature2;
    newCorrelation.corrlation = correlation;
    newCorrelation.lin_reg = linearReg;
    newCorrelation.threshold = maxDev * MULTIPLY_DEV;
    newCorrelation.mec = Circle(Point(0, 0), 0);
    newCorrelation.isByReg = true;
    //insert into cf (vector of correlatedFeatures) the new correlation
    cf.push_back(newCorrelation);

    // free allocations
    for (int i = 0; i < size; i++) {
        delete points[i];
    }
}

vector<pair<long, long>> SimpleAnomalyDetector::anomaliesByTimeStep(vector<AnomalyReport> reports) {
    vector<pair<long, long>> anomaliesByTimeStep;

    // check the vector of reports is not empty
    if (reports.empty()) {
        return anomaliesByTimeStep;
    }

    // initialize current description to be the first one
    string currentDescription = reports[0].description;
    pair<long, long> currentPair;
    // initialize the current pair to be the first time step
    currentPair.first = reports[0].timeStep;
    currentPair.second = reports[0].timeStep;

    for(int i = 1; i < reports.size(); i++) {
        // existing description. we need to check if it's a continuation of an old time step, or a new one
        if (currentDescription == reports[i].description) {
            // if we have a continues end time, it's the same anomaly, the same period of time
            if (currentPair.second + 1 == reports[i].timeStep) {
                // update the end time
                currentPair.second = reports[i].timeStep;

            // it's new period of time
            } else {
                // we add the current pair which we are done with
                anomaliesByTimeStep.push_back(currentPair);
                // initialize the pair to have the information of the new time step
                currentPair.first = reports[i].timeStep;
                currentPair.second = reports[i].timeStep;
            }

        // new description
        } else {
            // we add the current pair which we are done with
            anomaliesByTimeStep.push_back(currentPair);
            // updating the current description to be the new one
            currentDescription = reports[i].description;
            // initialize the pair to have the information of the new time step
            currentPair.first = reports[i].timeStep;
            currentPair.second = reports[i].timeStep;
        }
    }
    // add the final pair
    anomaliesByTimeStep.push_back(currentPair);
    return anomaliesByTimeStep;
}

