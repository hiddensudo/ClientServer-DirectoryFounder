#include "Server/Server.h"

int main() {
    Server server;
    server.createSocket();
    server.initializeServerAddress();
    server.bindingSocket();
    server.listetingSocket();

    while (true)
    {
        server.acceptingClient();
        server.run();
    }    
    return 0;
}