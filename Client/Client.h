#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#include "../PathFounder/PathFounder.h" // fix it in cmake

#define IP "127.0.0.2"
#define SERVER_MSG "CLIENT MESSAGE: "
#define SERVER_ERROR "CLIENT ERROR: "
#define CLOSE_CONNECTION_SYMBOL '#'

#define PATH_PREFIX "PATH:"
#define WANTED_FILE_PREFIX "WANTED:"

#define PORT 3000
#define BUFFER_SIZE 1024

class Client {
private:
    int client;
    bool isFound = false;
    bool isEnter = false;


    struct sockaddr_in serverAddress;

    char buffer[BUFFER_SIZE];

    std::string startPath;
    std::string wantedDir;
    std::string resultPath;


public:  
    Client();
    ~Client();

    void createSocket();
    void initializeClientAddress();
    void connectingToServer();
    void getMessage();
    void sendFullPath();
    void getPath(std::string& receivedMessage);
    void getWantedFileName(std::string& receivedMessage);
    void findPath();
    bool isConnectionClosed(const char* msg);
    void run();
};

#endif //CLIENT_H