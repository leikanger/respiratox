all: *.cpp
	make client
	make server

client: clientTCP.cpp
	g++ clientTCP.cpp -lboost_system -lboost_thread -lpthread  -o client.out

server: serverTCP.cpp
	g++ serverTCP.cpp -lboost_system -lboost_thread -lpthread -std=c++11 -o server.out 
