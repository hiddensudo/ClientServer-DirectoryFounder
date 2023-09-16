#include "Client/Client.h"

int main() {
    Client client;
    client.createSocket();
    client.initializeClientAddress();
    client.connectingToServer();
    client.run();
    
    return 0;
}