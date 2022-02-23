/*
 * HybridAnomalyDetector.cpp
 *
 * Author: Noa Eitan 316222777, Coral Kuta 208649186
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_
#include<iostream>
#include <cstring>
#include <fstream>
#include <utility>
#include <vector>
#include <sys/socket.h>
#include "HybridAnomalyDetector.h"

using namespace std;
#define COMMANDS_SIZE 6
#define READING_BUFFER_SIZE 2048

/**
 * class DataBase
 */
class DataBase{
    TimeSeries *train;
    TimeSeries *test;
    vector<AnomalyReport> anomalyReports;
    vector<pair<long, long>> reportsByTimeSteps;
    float threshold = 0.9;

public:
    /**
    * constructor - set members with nullptr
    */
    DataBase() {
        this->train = nullptr;
        this->test = nullptr;
    }

    /**
    * destructor - delete the members that we allocated
    */
    virtual ~DataBase(){
        delete(this->train);
        delete(this->test);
    }

    /**
    * getter of the train
    */
    TimeSeries* getTrain() {
        return this->train;
    }

    /**
    * getter of the test
    */
    TimeSeries* getTest() {
        return this->test;
    }

    /**
    * getter of the report
    */
    vector<AnomalyReport> getReports() {
        return this->anomalyReports;
    }

    /**
    * getter of the reports by timeSteps
    */
    vector<pair<long, long>> getReportsByTimeSteps() {
        return this->reportsByTimeSteps;
    }

    /**
    * set the train with new timeSeries
    */
    void setTrain(const char *nameTrain) {
        this->train = new TimeSeries(nameTrain);
    }

    /**
    * set the test with new timeSeries
    * @param nameTest the name of the file that we send to the constructor of timeSeries
    */
    void setTest(const char *nameTest) {
        this->test = new TimeSeries(nameTest);
    }

    /**
    * set the reports
    * @param reports the reports that we want to set in anomalyReports
    */
    void setReports(vector<AnomalyReport> reports) {
        this->anomalyReports = std::move(reports);
    }

    /**
    * set the timeSteps
    * @param reports the reports that we want to set in reportsByTimeSteps
    */
    void setTimeSteps(vector<pair<long, long>> reports) {
        this->reportsByTimeSteps = std::move(reports);
    }

    /**
    * getter of the threshold
    */
    float getThreshold() const {
        return this->threshold;
    }

    /**
    * set the threshold
    * @param newThreshold the new threshold
    */
    void setThreshold(float newThreshold) {
        this->threshold = newThreshold;
    }

};

/**
 * class DefaultIO
 * responsible of write and read
 */
class DefaultIO{
public:

    /**
    * read function
    * Return: string of what we read
    */
    virtual string read()=0;

    /**
    * write function
    * @param text - the text that we want to write
    */
    virtual void write(string text)=0;

    /**
    * write function
    * @param f, the float that we want to write
    */
    virtual void write(float f)=0;

    /**
    * read function
    * @param: f , the float that we want to read
    */
    virtual void read(float* f)=0;

    /**
    * function the uses write function and write to a file, until we see done
    */
    void uploadCSVFile(const string& fileName) {
        ofstream file;
        // open the file to write it
        file.open(fileName);
        if (file.is_open()) {
            string line;
            // read the file (as the io reads it) and write the text
            while ((line = read()) != "done") {
                file << line;
                file << "\n";
            }
            // close the file
            file.close();
        }
    }
    virtual ~DefaultIO(){}
};



class SocketIO: public DefaultIO{
    int clientID;

public:

    SocketIO(int id) {this->clientID = id;};

    /**
    * read function
    * Return: string of what we read
    */
    virtual string read() {
        char buffer = 0;
        recv(this->clientID, &buffer, sizeof (char), 0);
        string line;

        while (buffer != '\n'){
            line += buffer;
            recv(this->clientID, &buffer, sizeof (char), 0);
        }

        // ?????????????????????????
        //line += "\n";

        return line;
    }

    /**
    * write function
    * @param text - the text that we want to write
    */
    virtual void write(string text){
        const char* dataToSend = text.c_str();
        int length = text.length();
        send(this->clientID, dataToSend, length, 0);
    }

    /**
    * write function
    * @param f, the float that we want to write
    */
    virtual void write(float f){
        ostringstream stringStream;
        stringStream << f;
        string num(stringStream.str());
        write(num);

    }

    /**
    * read function
    * @param: f , the float that we want to read
    */
    virtual void read(float* f){
        *f = stof(read());
    }
};

/**
 * class standardIO
 */
class standardIO:public DefaultIO{
public:

    /**
    * read function
    * Return: string of what we read
    */
    virtual string read() {
        string s;
        cin>>s;
        return s;
    }

    /**
    * write function
    * @param text - the text that we want to write
    */
    virtual void write(string text){
        cout<<text<<endl;
    }

    /**
    * write function
    * @param f, the float that we want to write
    */
    virtual void write(float f){
        cout<<f<<endl;
    }

    /**
    * read function
    * @param: f , the float that we want to read
    */
    virtual void read(float* f){
        cin>>*f;
    }
};

/**
 * class command - a command that we want to execute
 */
class Command{
    string description;
    DefaultIO* dio;
    DataBase* database;
public:
    Command(DefaultIO* dio, string description, DataBase* data):
    dio(dio), description(description), database(data){}
    virtual void execute() = 0;
    virtual ~Command(){}
    // getter
    DefaultIO* getIO(){
        return dio;
    }
    // getter
    string getDescription(){
        return description;
    }

    DataBase* getDataBase(){
        return database;
    }
};


/**
 * UploadCSVFile - class that get details of 2 CSV files and insert the details into them
 */
class UploadCSVFile:public Command{
public:
    UploadCSVFile(DefaultIO *dio, string description, DataBase* data) : Command(dio, description, data){}
    void execute() override {
        // Train
        this->getIO()->write("Please upload your local train CSV file.\n");
        this->getIO()->uploadCSVFile("anomalyTrain.csv");
        this->getIO()->write("Upload complete.\n");

        // Test
        this->getIO()->write("Please upload your local test CSV file.\n");
        this->getIO()->uploadCSVFile("anomalyTest.csv");
        this->getIO()->write("Upload complete.\n");

        // set data with the CSV files
        this->getDataBase()->setTrain("anomalyTrain.csv");
        this->getDataBase()->setTest("anomalyTest.csv");
    }

};

/**
 * SetThreshold - class that set the threshold
 */
class SetThreshold:public Command{
public:
    SetThreshold(DefaultIO *dio, string description, DataBase* data) : Command(dio, description, data){}
    void execute() override{
        // get current correlation threshold
        float currThreshold = this->getDataBase()->getThreshold();

        this->getIO()->write("The current correlation threshold is ");
        this->getIO()->write(currThreshold);
        this->getIO()->write("\n");
        this->getIO()->write("Type a new threshold\n");

        // declare new threshold
        float newThreshold;

        // first get the new threshold and if it's good we can break, otherwise we print an error and try again.
        do {
            // get new threshold
            this->getIO()->read(&newThreshold);

            // if the new threshold is valid we break
            if ((newThreshold >= 0.5) && (newThreshold <= 1)) {
                break;
            }

            // if the new threshold is not valid we tell the client and return to the start
            this->getIO()->write("please choose a value between 0 and 1.");
        } while (true);

        // update the new threshold
        this->getDataBase()->setThreshold(newThreshold);
    }
};

/**
 * RunHybrid - class that run the hybrid algorithm on the csv files that we have - train and test
 */
class RunHybrid:public Command{
public:
    RunHybrid(DefaultIO *dio, string description, DataBase* data) : Command(dio, description, data){}
    virtual void execute(){
        // new hybrid anomaly detector
        auto* hybridAnomalyDetector = new HybridAnomalyDetector(this->getDataBase()->getThreshold());
        // learn normal data
        hybridAnomalyDetector->learnNormal(
                *this->getDataBase()->getTrain());
        // detect anomalies and save it in the database
        this->getDataBase()->setReports(hybridAnomalyDetector->detect(*this->getDataBase()->getTest()));
        this->getDataBase()->setTimeSteps(
                hybridAnomalyDetector->anomaliesByTimeStep(this->getDataBase()->getReports()));
        // print the client we finished
        this->getIO()->write("anomaly detection complete.\n");
        delete(hybridAnomalyDetector);
    }
};

/**
 * PrintAnomalies - class that print the anomalies that we found
 */
class PrintAnomalies:public Command{
public:
    PrintAnomalies(DefaultIO *dio, string description, DataBase* data) : Command(dio, description, data){}
    void execute() override{
        vector<AnomalyReport> anomalies = this->getDataBase()->getReports();

        // scan reports and write every timeStep
        for (AnomalyReport report : anomalies) {
            this->getIO()->write(report.timeStep);
            this->getIO()->write("\t");
            this->getIO()->write(report.description);
            this->getIO()->write("\n");
        }
        this->getIO()->write("Done.\n");
    }
};

/**
 * AnalyzeAnomalies - class that analyze our anomalies
 */
class AnalyzeAnomalies:public Command {
public:
    AnalyzeAnomalies(DefaultIO *dio, string description, DataBase* data) : Command(dio, description, data) {}

    void execute() override {
        this->getIO()->write("Please upload your local anomalies file.\n");

        // 2 vectors of pairs -> one for the blocks of reports of our test data, and one for the blocks from client
        vector<pair<long, long>> timeSteps;
        vector<pair<long, long>> anomalyReports = this->getDataBase()->getReportsByTimeSteps();

        string delimiter = ",";
        string token;
        size_t indexOfDelim;
        string line;
        // read the file (as the io reads it)
        while ((line = this->getIO()->read()) != "done") {
            pair<long, long> pair;
            // get the index of the delimiter ","
            indexOfDelim = line.find(delimiter);
            // insert the first time step into the pair
            pair.first = stol(line.substr(0, indexOfDelim));
            // erase the data until the delimiter
            line.erase(0, indexOfDelim + delimiter.length());
            // insert the second time step into the pair
            pair.second = stol(line);
            // insert the pair into the pairs vector
            timeSteps.push_back(pair);
        }
        this->getIO()->write("Upload complete.\n");

        // initial
        int positive = timeSteps.size();
        int negative = this->getDataBase()->getTest()->getSamplesNum();
        int falsePositive = 0;
        int truePositive = 0;
        long difference = 0;

        // count the rows that there was anomaly
        for (int i = 0; i < timeSteps.size(); i++) {
             difference += ((timeSteps[i].second - timeSteps[i].first) + 1);
        }
        // update negative
        negative -= int(difference);

        // scan every report and update FP -> by count the reports that do not intersect with anomaly
        for (int i = 0; i < anomalyReports.size(); i++) {
            int counter = 0;
            for (int j = 0; j < timeSteps.size(); j++) {

                // set start and end blocks time
                long startReport = anomalyReports[i].first;
                long endReport = anomalyReports[i].second;
                long startAnomaly = timeSteps[j].first;
                long endAnomaly = timeSteps[j].second;

                // if the report and the anomaly do not intersect -> count it
                if ((startReport > endAnomaly) || (startAnomaly > endReport)) {
                    counter++;
                }
            }

            // if the counter is equal the number of anomalies, the report do not intersect and anomaly
            if (counter == timeSteps.size()) {
                falsePositive++;
            }
        }

        // scan every anomaly and update TP -> by count the anomalies that do intersect with report
        for (int i = 0; i < timeSteps.size(); i++) {
            bool toAdd = false;
            for (int j = 0; j < anomalyReports.size(); j++) {

                // set start and end blocks time
                long startReport = anomalyReports[j].first;
                long endReport = anomalyReports[j].second;
                long startAnomaly = timeSteps[i].first;
                long endAnomaly = timeSteps[i].second;

                // if they are intersected -> turn the flag on.
                if ((endReport >= startAnomaly) && (endAnomaly >= startReport)) {
                    toAdd = true;
                    break;
                }
            }

            // there was an intersection
            if (toAdd) {
                truePositive++;
            }
        }
        auto truePositiveRate = float(((int) (1000.0 * truePositive / positive))/1000.0f);
        auto falsePositiveRate = float(((int) (1000.0 * falsePositive / negative))/1000.0f);

        this->getIO()->write("True Positive Rate: ");
        this->getIO()->write(truePositiveRate);
        this->getIO()->write("\n");
        this->getIO()->write("False Positive Rate: ");
        this->getIO()->write(falsePositiveRate);
        this->getIO()->write("\n");
    }
};

/**
 * Exit class
 */
class Exit : public Command {
public:
    Exit(DefaultIO *dio, string description, DataBase* data) : Command(dio, description, data) {}

    void execute() override {
    }
};


/**
 * Menu class - class that print the menu by printing the description of every command.
 */
class Menu : public Command {
    Command **commands = new Command *[6];
public:

    /**
    * constructor
    */
    Menu(DefaultIO *dio, DataBase* data) :
    // call super of menu command and initialize with commands
            Command(dio, "Welcome to the Anomaly Detection Server.\nPlease choose an option:\n", data) {
        commands[0] = new UploadCSVFile(dio, "1.upload a time series csv file\n", data);
        commands[1] = new SetThreshold(dio, "2.algorithm settings\n", data);
        commands[2] = new RunHybrid(dio, "3.detect anomalies\n", data);
        commands[3] = new PrintAnomalies(dio, "4.display results\n", data);
        commands[4] = new AnalyzeAnomalies(dio, "5.upload anomalies and analyze results\n", data);
        commands[5] = new Exit(dio, "6.exit\n", data);
    }
    void execute() override {
        // print the beginning of the menu.
        this->getIO()->write(this->getDescription());
        // size of commands
        int commandsSize = COMMANDS_SIZE;
        // print the menu
        for (int i = 0; i < commandsSize; i++) {
            // print the description of each command
            commands[i]->getIO()->write(commands[i]->getDescription());
        }
    }
    Command **getMenu() {
        return commands;
    }
};

#endif /* COMMANDS_H_ */