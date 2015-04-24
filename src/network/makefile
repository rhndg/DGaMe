all: network.h network.cpp 
	g++ -g -std=c++11 -w network.h network.cpp -pthread

debug: all
	g++ -g -std=c++11 -Wall network.h network.cpp -lpthread ;\
	gdb ./a.out