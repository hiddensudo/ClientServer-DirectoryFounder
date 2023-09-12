all: server client

server:
	g++ server.cpp -o server

client: finpath/findPath.cpp client.cpp
	g++ finpath/findPath.cpp client.cpp -o client

server_run:
	./server

client_run:
	./client
