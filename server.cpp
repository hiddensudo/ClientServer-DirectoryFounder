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

#define SERVER_MSG "SERVER MESSAGE: "
#define SERVER_ERROR "SERVER ERROR: "
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

void sendPath(int server, const std::string& messagePrefix) {
    char buffer[BUFFER_SIZE];
    
    std::cout << "Enter " << messagePrefix << " directory" << std::endl;
    std::cout << ">>> ";
    std::cin.getline(buffer, BUFFER_SIZE);
    
    std::string inputString = buffer;
    std::string resultString = messagePrefix + inputString;
    
    // Копіюємо рядок у буфер та надсилаємо його на сервер
    std::strncpy(buffer, resultString.c_str(), sizeof(buffer));
    send(server, buffer, BUFFER_SIZE, 0);
}


int main() {
    //Create socket
    int client;
    int server;
    std::string inputString;
    std::string resultString;

    struct sockaddr_in server_address;

    client = socket(AF_INET, SOCK_STREAM, 0);
    if(client < 0) {
        std::cout << SERVER_ERROR << "establishing socket error" << std::endl;
        exit(1);
    }

    std::cout << SERVER_MSG << "Server socket was successfully created" << std::endl;

    server_address.sin_port = htons(PORT);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htons(INADDR_ANY);

    //binding socket
    if(bind(client, reinterpret_cast<struct sockaddr*>(&server_address), sizeof(server_address)) < 0) {
        std::cout << SERVER_ERROR << "binding connection" << std::endl;
        exit(1);
    }

    //listening socket
    socklen_t size = sizeof(server_address);
    std::cout << SERVER_MSG << "Listening clients..." << std::endl;
    listen(client, 1);

    server = accept(client, reinterpret_cast<struct sockaddr*>(&server_address), &size);
    if(server < 0) {
        std::cout << SERVER_ERROR << "Cannot accepting client" << std::endl;
    }

    char buffer[BUFFER_SIZE];
    bool is_exit = false;
    while(server > 0) {
        strcpy(buffer, "Server connected");
        send(server, buffer, BUFFER_SIZE, 0);
        std::cout << SERVER_MSG << "Connected to the client" << std::endl 
            << "Enter " << CLOSE_CONNECTION_SYMBOL << " to end the connection" << std::endl;

        if(close_connection(buffer)) {
            is_exit = true;
        }

        while(!is_exit) {
            //send message from server to client
            std::cout << ">>> ";
            std::cin.getline(buffer, BUFFER_SIZE);
            std::string receivedMessage(buffer);
            if(close_connection(buffer)) {
                send(server, buffer, BUFFER_SIZE, 0);
                break;
            }
            
            if(receivedMessage.find("/getpath") == 0) {
                sendPath(server, PATH_PREFIX);
                sendPath(server, WANTED_FILE_PREFIX);
            }
            
            receivedMessage = "";
            //get message from client to server
            /*
            recv(server, buffer, BUFFER_SIZE, 0);
            if(close_connection(buffer)) {
                break;
            }
            std::cout << "Client: "; 
            std::cout << buffer;
            */
        }

        std::cout << "\nConnection closed!" << std::endl;
        is_exit = false;
    }

    close(server);
    return 0;
}