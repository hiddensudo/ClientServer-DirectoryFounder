#include "Client.h"

void Client::createSocket() {
    this->client = socket(AF_INET, SOCK_STREAM, 0);
    if(this->client < 0) {
        std::cout << SERVER_ERROR << "establishing socket error";
        exit(1);
    }
}

void Client::initializeClientAddress() {
    this->serverAddress.sin_port = htons(PORT);
    this->serverAddress.sin_family = AF_INET;
    inet_pton(AF_INET, IP, &serverAddress.sin_addr);
}

void Client::connectingToServer() {
    if(connect(this->client,reinterpret_cast<struct sockaddr*>(&this->serverAddress), sizeof(this->serverAddress)) == 0) {
        std::cout << SERVER_MSG << "Connection to server " << inet_ntoa(this->serverAddress.sin_addr)
                << " with port number: " << PORT << std::endl;
    }
}

bool Client::isConnectionClosed(const char* msg) {
    for(int i = 0; i < strlen(msg); ++i) {
        if(msg[i] == CLOSE_CONNECTION_SYMBOL) {
            return true;
        }
    }
    return false;
}

void Client::getMessage() {
    recv(this->client, this->buffer, BUFFER_SIZE, 0);
        if(isConnectionClosed(this->buffer)) {
            this->isEnter = true;
        }
    std::cout << "Server: " << this->buffer << std::endl;
}

void Client::sendFullPath() {
    std::strncpy(this->buffer, this->resultPath.c_str(), sizeof(this->buffer));
    send(this->client, this->buffer, BUFFER_SIZE, 0);
}


void Client::getPath(std::string& receivedMessage) {
    if (receivedMessage.find(PATH_PREFIX) == 0) {
        size_t pos = receivedMessage.find(':');
        this->startPath = receivedMessage.substr(pos+1);
    }
}

void Client::findPath() {
    PathFounder f(this->startPath, this->wantedDir);
    f.run(this->startPath);
    this->resultPath = f.getResultPath();
    sendFullPath();
}
 
void Client::getWantedFileName(std::string& receivedMessage) {
    if(receivedMessage.find(WANTED_FILE_PREFIX) == 0) {
        size_t pos = receivedMessage.find(':');
        this->wantedDir = receivedMessage.substr(pos+1);
        
        findPath();

        std::cout << std::endl;
    }
}

void Client::run() {

    while(!this->isEnter) {
        getMessage();

        if (isConnectionClosed(this->buffer)) {
            std::cout << "Connection closed by server." << std::endl;
            break;
        }

        std::string receivedMessage(this->buffer);

        getPath(receivedMessage);

        getWantedFileName(receivedMessage);
    }

    std::cout << "Connection closed" << std::endl;
    this->isEnter = false;
}

Client::Client() {}

Client::~Client() {}