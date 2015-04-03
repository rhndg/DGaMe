game_map_h = game_map/game_map.h game_map/ai.h game_map/draw.h game_map/update.h 
game_map_cpp = game_map/game_map.cpp game_map/draw.cpp game_map/update.cpp


CXX = g++ -std=c++11 -w
DRAWFLAGS = -lGL -lGLU -lglut
THREADFLAGS = -lpthread
DEBUG = -DTO_DEBUG

C_ALL = $(CXX) $(DRAWFLAGS) $(THREADFLAGS)

all: main/main.cpp vector.o player.o bullet.o grid.o game_map.o globals.o network.o thread_functions.o
	$(C_ALL) -o Game main/main.cpp

vector.o: vector/Vector3.h vector/Vector3.cpp
	$(CXX) -c vector/Vector3.cpp
player.o: player/player.h player/player.cpp vector.o
	$(CXX) -c player/player.cpp
bullet.o: bullet/bullet.h bullet/bullet.cpp vector.o
	$(CXX) -c bullet/bullet.cpp
grid.o: grid/grid.h grid/grid.cpp vector.o
	$(CXX) -c grid/grid.cpp
game_map.o: $(game_map_h) $(game_map_cpp) player.o grid.o bullet.o vector.o
	$(CXX) $(DRAWFLAGS) -c $(game_map_cpp)
globals.o: main/globals.h main/globals.cpp game_map.o vector.o
	$(CXX) $(DRAWFLAGS) -c main/globals.cpp
network.o: network/network.h network/network.cpp game_map.o globals.o
	$(C_ALL) -c network/network.cpp
thread_functions.o: main/thread_functions.h main/thread_functions.cpp network.o
	$(C_ALL) -c main/thread_functions.cpp


clean:
	rm -f *.o

join_game: all
	./Game join $(PEER)

host_game: all
	./Game host $(PORT)

debug:
	