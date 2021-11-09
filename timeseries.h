/*
 * timeseries.h
 *
 * Author: Noa Eitan 316222777, Coral Kuta 208649186
 */

#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <map>
#include <algorithm>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <string>

using namespace std;

class TimeSeries {
    // FIELDS:
    // a vector of vectors. each mini vector has the values of one LINE ! Each line is one sample.
    vector<vector<float>> _tableByLines;
    // a vector of vectors. each mini vector has the values of one COLUMN ! Each column is one feature.
    vector<vector<float>> _tableByColumns;
    // a vector of the features names.
    vector<string> _featureNames;
    // how many features we have.
    int _featureNum;
    // how many samples we have.
    int _samplesNum;

public:
    TimeSeries(const char *CSVfileName);
    const vector<vector<float>> getFeatures() const;
    const vector<vector<float>> getSamples() const;
    const string getFeatureName(int index) const;
    int getFeatureCol(const string& featureName) const;
    vector<vector<float>> extractDataFeatures(const char *CSVfileName);
    vector<vector<float>> extractDataSamples(const char *CSVfileName);
    vector<string> extractFeatureNames(const char *CSVfileName);
    float* getFeatureColArr(int index);
};

#endif /* TIMESERIES_H_ */