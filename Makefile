all: server client

server:
	g++ server.cpp -o server

client: PathFounder/PathFounder.cpp Client/Client.cpp mainClient.cpp
	g++ PathFounder/PathFounder.cpp  Client/Client.cpp mainClient.cpp -o client

server_run:
	./server

client_run:
	./client

server: Server/Server.cpp mainServer.cpp
	g++ Server/Server.cpp mainServer.cpp -o server

clear:
	rm -rf client server