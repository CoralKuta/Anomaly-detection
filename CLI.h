/*
 * HybridAnomalyDetector.cpp
 *
 * Author: Noa Eitan 316222777, Coral Kuta 208649186
 */

#ifndef CLI_H_
#define CLI_H_

#include <cstring>
#include "commands.h"

using namespace std;

class CLI {
	DefaultIO* dio;
    Menu* menu;
    DataBase* data;
public:

    /**
    * constructor - set members
    * @param dio a pointer to a default dio
    */
	CLI(DefaultIO* dio);
    /**
    * start method - method that run menu and execute commands, by the user choice. end when the user choose option 6
    */
	void start();

    /**
    * destructor
    */
	virtual ~CLI();
};

#endif /* CLI_H_ */
