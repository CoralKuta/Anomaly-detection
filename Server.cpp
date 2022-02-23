#include "Server.h"


Server::Server(int port)throw(const char*) {
    this->port = port;
    this->ip = "127.0.0.1";
    // create a new IPV4 tcp socket.
    this->socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    // check that the socket could open.
    if (this->socket_fd < 0) {
        throw("couldn't open a new socket");
    }

    // prepare the sockaddr_in struct with the server's socket address.
    this->serverAddress.sin_family = AF_INET;
    // convert port to network order
    this->serverAddress.sin_port = htons(port);

    // check that the ip is valid.
    if (inet_aton(ip.c_str(), &serverAddress.sin_addr) == 0) {
        throw("ip address is not valid.");
    }

    // try to bind and check the binding worked.
    if (bind(socket_fd, (const sockaddr *)&serverAddress, sizeof (serverAddress)) < 0) {
        throw("Call to bind() failed.");
    }
}

void sig_handler(int signum){
    printf("Inside handler function\n");
}

void Server::start(ClientHandler& ch) throw(const char*){

    this->t = new thread([&ch, this]() {

        int client_socket_fd;

        if (this->socket_fd < 0) {
            throw("No socket.");
        }

        // struct for the client Address and get its length.
        struct sockaddr_in clientAddress;
        socklen_t clientAddressLen = sizeof(clientAddress);

        //listen to a new client.
        if (listen(this->socket_fd, clientLimit()) < 0) {
            throw("Couldn't connect to a new client");
        }

        signal(SIGALRM, sig_handler);

        while (!this->shouldStop) {

            alarm(1);
            // create a socket for a specific client
            if ((client_socket_fd = accept(this->socket_fd, (sockaddr *)&clientAddress, &clientAddressLen)) < 0) {
                if (errno == EINTR) {
                    fprintf(stderr, "socket timout");
                }
                //throw("Couldn't accept a new client");
            }
            alarm(0);


            ch.handle(client_socket_fd);

            close(client_socket_fd);
        }
    });
}

void Server::stop(){
    this->shouldStop = true;
    // The main thread waits for thread t to finish
	t->join();
}

Server::~Server() {
    if (close(socket_fd) < 0) {
        cerr << "ERROR" << strerror(errno) << endl;
    }
}

int Server::clientLimit() {
    return MAX_CLIENTS;
}




//#include <csignal>
//#include <signal.h>
//#include "Server.h"
//
//Server::Server(int port)throw (const char*) {
//    //tcp communication with ipv4 addresses.
//    this->server_socket = socket(AF_INET, SOCK_STREAM, 0);
//    if(this->server_socket < 0)
//        throw("Socket Failed");
//    this->stop_server = false;
//    this->my_addr.sin_family = AF_INET;
//    this->my_addr.sin_port = htons(port);
//    this->my_addr.sin_addr.s_addr = INADDR_ANY;
//    this->client_size = sizeof(my_addr);
//    if(bind(server_socket, (sockaddr*)&my_addr, this->client_size)<0){
//        throw("bind failed");
//    }
//}
//void sig_handler(int signum){
//    printf("Inside handler function\n");
//}
//void Server::start(ClientHandler& ch)throw(const char*){
//    //create a new thread with this pointer and the client handler for it to deal with clients.
//    this->t = new thread([&ch, this](){
//        //set the maximum number of clients to 3.
//        if(listen(server_socket, 3)<0){
//            throw("listen failed");
//        }
//        signal(SIGALRM , sig_handler);
//        while(!stop_server){
//            //maybe set time out to this accept, if the condition was true but there is no other client actually.
//            alarm(1);
//            int client_ID = accept(this->server_socket,(sockaddr*)&this->my_addr, &this->client_size);
//            if(client_ID>0){
//                ch.handle(client_ID);
//                close(client_ID);
//            } else{
//                throw("accept failed");
//            }
//            alarm(0);
//        }
//        //close the server socket when finished with all clients- stop listening in this port.
//        close(server_socket);
//    });
//}
//
//void Server::stop(){
//    this->stop_server = true;
//    t->join(); // do not delete this!
//}
//
//Server::~Server() {
//}
