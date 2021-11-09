/*
 * anomaly_detection_util.cpp
 *
 * Author: Noa Eitan 316222777, Coral Kuta 208649186
 */

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>
#include <map>
#include <list>
#include <iterator>
const float THRESHOLD = 0.9;


struct correlatedFeatures{
    // names of the correlated features
	string feature1,feature2;
	float corrlation;
	Line lin_reg;
	float threshold;
};


class SimpleAnomalyDetector:public TimeSeriesAnomalyDetector{
	vector<correlatedFeatures> cf;
public:

    /**
     * function: SimpleAnomalyDetector
     * constructor.
     */
	SimpleAnomalyDetector();

    /**
    * function: ~SimpleAnomalyDetector
    * destructor.
    */
	virtual ~SimpleAnomalyDetector();

    /**
     * function: learnNormal.
     * @param ts a TimeSeries object.
     * The function learn a normal data from ts and add corallation features to cf member.
     */
	virtual void learnNormal(const TimeSeries& ts);

    /**
     * function: detect.
     * @param ts a TimeSeries object.
     * The function scan data from ts and add report deviations, by the correlations from learnNormal.
     */
	virtual vector<AnomalyReport> detect(const TimeSeries& ts);

    /**
     * function: getNormalModel
     * @param ts a TimeSeries object.
     * @return vector of correlatedFeatures
     * The function return the correlated features in the data.
     */
	vector<correlatedFeatures> getNormalModel(){
		return cf;
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
    virtual void addCorrelation(string feature1, string feature2, float *x, float *y, int size, float correlation);
};

#endif /* SIMPLEANOMALYDETECTOR_H_ */
