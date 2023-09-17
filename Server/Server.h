#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <string>
#include <string.h>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <thread>
#include <vector>

#define SERVER_MSG "SERVER MESSAGE: "
#define SERVER_ERROR "SERVER ERROR: "
#define CLOSE_CONNECTION_SYMBOL '#'
#define KILL_SERVER_SYMBOL 'y'
#define KEEP_SERVER_SYMBOL 'n'

#define PATH_PREFIX "PATH:"
#define WANTED_FILE_PREFIX "WANTED:"

#define PORT 3000
#define BUFFER_SIZE 1024

class Server
{
private:
    int client;
    int server;
    bool isExit;

    struct sockaddr_in serverAddress;
    socklen_t size;

    char buffer[BUFFER_SIZE];
    
public:
    Server();
    ~Server();

    void createSocket();
    void initializeServerAddress();
    void bindingSocket();
    void listeningSocket();
    void acceptingClient();
    void getMessage();
    void sendMessage();
    bool isConnectionClosed(const char* msg);
    bool isServerKilled(const char* msg);
    void killServer();
    void run();

    void sendPath(const std::string& messagePrefix);

};

#endif //SERVER_H