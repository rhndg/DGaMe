all: network.h network.cpp 
	g++ -std=c++11 network.h network.cpp -lpthread

debug: all
	g++ -g -std=c++11 network.h network.cpp -lpthread ;\
	gdb ./a.out