/*
 * Server.h
 *
 *  Created on: Dec 13, 2020
 *      Author: Eli
 */

#ifndef SERVER_H_
#define SERVER_H_
#include "commands.h"
#include "CLI.h"
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <thread>
#include <signal.h>
#include <pthread.h>
#include <exception>
#include <csignal>


#define MAX_CLIENTS 3


using namespace std;

// edit your ClientHandler interface here:
// interface
class ClientHandler{
    public:
    virtual void handle(int clientID)=0;
};


// you can add helper classes here and implement on the cpp file


// edit your AnomalyDetectionHandler class here
// mas o menos como el main que tubivmos en el ass anterior.
// handle one client, open socket io and cli.
// the server opened a socket. he has id, we did accept. open socket io
class AnomalyDetectionHandler:public ClientHandler{
	public:
    virtual void handle(int clientID){
        DefaultIO* dio = new SocketIO(clientID);
        CLI* cli = new CLI(dio);
        cli->start();
    }
};


// implement on Server.cpp
class Server {
    // the thread to run the start() method in
	thread* t;
    string ip;
    int port, socket_fd;
    struct sockaddr_in serverAddress;
    bool shouldStop = false;



public:
	Server(int port) throw(const char*);
	virtual ~Server();
    int clientLimit();
	void start(ClientHandler& ch)throw(const char*);
	void stop();
};

#endif /* SERVER_H_ */
