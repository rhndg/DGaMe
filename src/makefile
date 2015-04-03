game_map_h = game_map.h ai.h draw.h update.h 
game_map_cpp = game_map.cpp draw.cpp update.cpp


CXX = g++ -std=c++11 -w
DRAWFLAGS = -lGL -lGLU -lglut
THREADFLAGS = -lpthread
DEBUG = -DTO_DEBUG

C_ALL = $(CXX) $(DRAWFLAGS) $(THREADFLAGS)

all: main.cpp player bullet grid game_map globals network thread_functions
	$(C_ALL) -o Game main.cpp

player.o: player.h player.cpp
	$(CXX) -c player.cpp
bullet.o: bullet.h bullet.cpp
	$(CXX) -c bullet.cpp
grid.o: grid.h grid.cpp
	$(CXX) -c grid.cpp
game_map.o: $(game_map_h) $(game_map_cpp) player.o grid.o bullet.o
	$(CXX) $(DRAWFLAGS) -c $(game_map_cpp)
globals.o: globals.h globals.cpp game_map.o
	$(CXX) $(DRAWFLAGS) -c globals.cpp
network.o: network.h network.cpp game_map.o globals.o
	$(C_ALL) network.cpp
thread_functions.o:thread_functions.h thread_functions.cpp network.o
	$(C_ALL) thread_functions.cpp


clean:
	rm -f *.o

join_game: all
	./Game join $(PEER)

host_game: all
	./Game host $(PORT)

debug:
	