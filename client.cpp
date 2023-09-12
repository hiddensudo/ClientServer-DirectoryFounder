#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#include "PathFounder/PathFounder.h"

#define IP "127.0.0.1"
#define SERVER_MSG "CLIENT MESSAGE: "
#define SERVER_ERROR "CLIENT ERROR: "
#define CLOSE_CONNECTION_SYMBOL '#'

#define PATH_PREFIX "PATH:"
#define WANTED_FILE_PREFIX "WANTED:"

#define PORT 3001
#define BUFFER_SIZE 1024

//function check if client close connection
bool close_connection(const char* msg) {
    for(int i = 0; i < strlen(msg); ++i) {
        if(msg[i] == CLOSE_CONNECTION_SYMBOL) {
            return true;
        }
        return false;
    }
}


int main() {
    int client;
    struct sockaddr_in server_address;

    client = socket(AF_INET, SOCK_STREAM, 0);
    if(client < 0) {
        std::cout << SERVER_ERROR << "establishing socket error";
        exit(1);
    }

    server_address.sin_port = htons(PORT);
    server_address.sin_family = AF_INET;
    inet_pton(AF_INET, IP, &server_address.sin_addr);

    std::cout << "Client socket created" << std::endl;

    if(connect(client,reinterpret_cast<struct sockaddr*>(&server_address), sizeof(server_address)) == 0) {
        std::cout << SERVER_MSG << "Connection to server " << inet_ntoa(server_address.sin_addr)
                << " with port number: " << PORT << std::endl;
    }

    char buffer[BUFFER_SIZE];
    std::cout << SERVER_MSG << "Waiting for server confirmation..." << std::endl;
    std::cout << "Connection established." << std::endl
            << "Enter " << CLOSE_CONNECTION_SYMBOL << " for close the connection" << std::endl;
    
    std::string startPath;
    std::string wantedDir;

    bool is_enter = false;
    while(!is_enter) {
        //send message from client to server
        
        //std::cout << ">>> ";
        //std::cin.getline(buffer, BUFFER_SIZE);
        //send(client, buffer, BUFFER_SIZE, 0);
        //if(close_connection(buffer)) {
        //    break;
        //}


        //get message from server to client
        recv(client, buffer, BUFFER_SIZE, 0);
        if(close_connection(buffer)) {
            is_enter = true;
        }
        std::cout << "Server: " << buffer << std::endl;

        std::string receivedMessage(buffer);
        if (receivedMessage.find(PATH_PREFIX) == 0) {
            size_t pos = receivedMessage.find(':');

            startPath = receivedMessage.substr(pos+1);
        }

        if(receivedMessage.find(WANTED_FILE_PREFIX) == 0) {
            size_t pos = receivedMessage.find(':');
            wantedDir = receivedMessage.substr(pos+1);
            PathFounder f(startPath, wantedDir);
            f.processDirectory(startPath, wantedDir);
            std::cout << std::endl;
        }
    }

    std::cout << "Connection closed" << std::endl;
    is_enter = false;
    close(client);
    return 0;
}