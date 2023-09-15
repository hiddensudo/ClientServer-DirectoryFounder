all: server client

server:
	g++ server.cpp -o server

client: PathFounder/PathFounder.cpp client.cpp
	g++ PathFounder/PathFounder.cpp client.cpp -o client

server_run:
	./server

client_run:
	./client
 