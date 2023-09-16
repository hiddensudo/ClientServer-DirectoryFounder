#include "Server.h"

void Server::createSocket() {
    this->client = socket(AF_INET, SOCK_STREAM, 0);
    if(client < 0) {
        std::cout << SERVER_ERROR << "establishing socket error" << std::endl;
        exit(1);
    }

    std::cout << SERVER_MSG << "Server socket was successfully created" << std::endl;

}

void Server::initializeServerAddress() {
    this->serverAddress.sin_port = htons(PORT);
    this->serverAddress.sin_family = AF_INET;
    this->serverAddress.sin_addr.s_addr = htons(INADDR_ANY);
}

void Server::bindingSocket() {
    if(bind(this->client, reinterpret_cast<struct sockaddr*>(&this->serverAddress), sizeof(this->serverAddress)) < 0) {
        std::cout << SERVER_ERROR << "binding connection" << std::endl;
        exit(1);
    }
}

void Server::listetingSocket() {
    this->size = sizeof(serverAddress);
    std::cout << SERVER_MSG << "Listening clients..." << std::endl;
    listen(client, 1);
}

void Server::acceptingClient() {
    this->server = accept(this->client, reinterpret_cast<struct sockaddr*>(&this->serverAddress), &this->size);
    if(this->server < 0) {
        std::cout << SERVER_ERROR << "Cannot accepting client" << std::endl;
    }
}

void Server::sendPath(const std::string& messagePrefix) {
    char buffer[BUFFER_SIZE];
    
    std::cout << "Enter " << messagePrefix << " directory" << std::endl;
    std::cout << ">>> ";
    std::cin.getline(this->buffer, BUFFER_SIZE);
    
    std::string inputString = this->buffer;
    std::string resultString = messagePrefix + inputString;
    
    std::strncpy(this->buffer, resultString.c_str(), sizeof(this->buffer));
    send(this->server, this->buffer, BUFFER_SIZE, 0);
}

void Server::sendMessage() {
    std::cout << ">>> ";
    std::cin.getline(this->buffer, BUFFER_SIZE);
    if(isConnectionClosed(this->buffer)) {
        send(this->server, this->buffer, BUFFER_SIZE, 0);
        this->isExit = true;
    }
}

void Server::getMessage() {
    recv(this->server, this->buffer, BUFFER_SIZE, 0);
    if(isConnectionClosed(this->buffer)) {
        this->isExit = true;
    }
    std::cout << "Client: "; 
    std::cout << this->buffer;
}

bool Server::isConnectionClosed(const char* msg) {
    for(int i = 0; i < strlen(msg); ++i) {
        if(msg[i] == CLOSE_CONNECTION_SYMBOL) {
            return true;
        }
        return false;
    }
}

void Server::run() {
    while(this->server > 0) {
        strcpy(this->buffer, "Server connected");
        send(this->server, this->buffer, BUFFER_SIZE, 0);
        std::cout << SERVER_MSG << "Connected to the client" << std::endl 
            << "Enter " << CLOSE_CONNECTION_SYMBOL << " to end the connection" << std::endl;


        if(isConnectionClosed(this->buffer)) {
            break;
        }

        while(!this->isExit) {
            //send message to client
            sendMessage();

            std::string receivedMessage(this->buffer);

            if(receivedMessage.find("/getpath") == 0) {
                sendPath(PATH_PREFIX);
                sendPath(WANTED_FILE_PREFIX);
                //getMessage();
            }
            
            //receivedMessage.clear();

            //get message from client;
            //getMessage();
        }

        std::cout << "\nConnection closed!" << std::endl;
        this->isExit = false;

        acceptingClient();
    }
    close(this->client);

    close(this->server); 
}

Server::Server() {
    this->isExit = false;
}

Server::~Server() {};