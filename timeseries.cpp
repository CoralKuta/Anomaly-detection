/*
 * anomaly_detection_util.cpp
 *
 * Author: Noa Eitan 316222777, Coral Kuta 208649186
 */

#include "timeseries.h"

//constructor
TimeSeries::TimeSeries(const char *CSVfileName) {
        _tableByLines = extractDataSamples(CSVfileName);
        _featureNames = extractFeatureNames(CSVfileName);
        _featureNum = _featureNames.size();
        _samplesNum = _tableByLines.size();
        _tableByColumns = extractDataFeatures(CSVfileName);
}

/**
*
* @return a vector of vectors. each mini vector has the values of one column (one feature).
*/
const vector<vector<float>> TimeSeries::getFeatures() const {
    return _tableByColumns;
}

/**
*
* @return the table - a vector of vectors. Each mini vector is one line of the table (one sample).
*/
const vector<vector<float>> TimeSeries::getSamples() const {
    return _tableByLines;
}

/**
 * Given an index, the function returns the name of the right feature.
 * @param index the index of the feature's name desired
 * @return the name of the feature. Error if there is no such index.
 */
const string TimeSeries::getFeatureName(int index) const {
    if ((index >= _featureNames.size()) || (_featureNames.empty())){
        return "Error !";
    }
    return _featureNames[index];
}


/**
 * Given the name of the feature, the function returns the index of feature column.
 * @param featureName
 * @return index i if the feature exists, -1 if it doesn't.
 */
int TimeSeries::getFeatureCol(const string& featureName) const {
    for (int i = 0; i < _featureNames.size(); i++) {
        if (featureName == _featureNames[i]) {
            return i;
        }
    }
    return -1;
}

// TODO !
//    /**
//     *
//     * @param feature
//     * @param time
//     * @return
//     */
//    float valueOfFeatureOnAGivenTime(string feature, float time) {
//    // ???????????????????????
//    }


/**
 * Given a csv file, we extract the file's data by columns (features), and store the data in a data base.
 * @param CSVfileName
 * @return the data base - a vector of vectors. each mini vector has the values of one column (one feature).
 */
vector<vector<float>> TimeSeries::extractDataFeatures(const char *CSVfileName) {
    vector<vector<float>> data = extractDataSamples(CSVfileName);

    vector<vector<float>> tableByColumns;
    for (int i = 0; i < _featureNum; i++) {
        // for every feature we have we create a vector
        vector<float> feature;
        for (auto line : data) {
            // we go through all the lines and store the i element of each of them in the vector.
            feature.push_back(line[i]);
        }
        // store the feature (column)
        tableByColumns.push_back(feature);
    }
    return tableByColumns;
}


/**
 * Given a csv file, we extract the file's data by lines (samples), and store the data in a data base.
 * @param CSVfileName
 * @return the data base - a vector of vectors. each mini vector has the values of one line (one sample).
 */
vector<vector<float>> TimeSeries::extractDataSamples(const char *CSVfileName) {
    // creating a table to store data.
    vector<vector<float>> tableByLines;
    // stream class to read from the gives csv file.
    ifstream file(CSVfileName);

    // ignoring the first line of the file because it has the feature's names and not actual data.
    string line;
    getline(file, line);

    // while the file is not empty
    while (file) {
        // if there is no line to read we break from the loop and exit.
        if (! getline(file, line)){
            break;
        }

        // if the line we read is not empty we begin to scan it.
        if (line[0] != '\n') {
            // we move the data from the line to "iss".
            istringstream iss(line);
            // creating a single row vector to store the data from one row (sample)
            vector<float> sample;

            // while the line is not empty we split it by commas.
            while (iss) {
                // getting one value from the line
                string value;
                // if the split didn't succeed we break.
                if (! getline(iss, value, ','))
                    break;
                try {
                    // we insert the value to the specific sample vector.
                    sample.push_back(stof(value));
                }
                catch (const std::invalid_argument e) {
                    cout << "NaN found in file " << CSVfileName << endl;
                    e.what();
                }
            }
            // returning the table.
            tableByLines.push_back(sample);
        }
    }

    if (!file.eof()) {
        cerr << "Could not read file " << CSVfileName << "\n";
        __throw_invalid_argument("File not found.");
    }

    file.close();
    return tableByLines;
}


// TODO NOTES
/**
 * Given a csv file, we extract the first line - the feature's names.
 * @param CSVfileName
 * @return a vector of strings. each string is a name of one feature.
 */
vector<string> TimeSeries::extractFeatureNames(const char *CSVfileName) {
    vector<string> features;
    ifstream file(CSVfileName);
    int lineNum = 1;
    string str;

    while (file) {
        if (!getline(file, str)) {
            break;
        }

        istringstream ss(str);

        while (ss) {
            string line;
            getline(ss, line, ',');

            // we want to read only the first line of features.
            if (lineNum != 1) {
                break;
            }

            //remove spaces and store in the feature vector.
            line.erase(remove(line.begin(), line.end(), ' '), line.end());
            if (line == "") {
                break;
            }
            features.push_back(line);
        }

        // increase number of line to know when we need to stop reading.
        lineNum++;
    }
    return features;
}


