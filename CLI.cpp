/*
 * HybridAnomalyDetector.cpp
 *
 * Author: Noa Eitan 316222777, Coral Kuta 208649186
 */

#include "CLI.h"
#include <string>

/**
 * constructor - set members
 * @param dio a pointer to a default dio
 */
CLI::CLI(DefaultIO* dio) {
    this->dio = dio;
    this->data = new DataBase();
    this->menu = new Menu(this->dio, data);

}

/**
 * start method - method that run menu and execute commands, by the user choice. end when the user choose option 6
 */
void CLI::start(){
    Command** commands = this->menu->getMenu();
    // a loop that executes while stop is false
    while (true) {
        // print the menu to the client
        this->menu->execute();
        // read the client's option
        string option = this->dio->read();
        // casting to int
        int intOption = std::stoi(option) - 1;
        // execute the wanted command
        commands[intOption]->execute();

        // if option is 6 - stop
        if (option == "6") {
            break;
        }
    }
}

/**
 * destructor
 */
CLI::~CLI() {
    delete(this->menu);
    delete(this->data);
}