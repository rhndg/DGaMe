graphics_cpp = graphics/spark.cpp graphics/camera.cpp graphics/lodepng.cpp graphics/image.cpp vector/Vector3.cpp grid/grid.cpp game_map/game_map.cpp main/globals.cpp
graphics_h = graphics/spark.h graphics/camera.h graphics/lodepng.h graphics/image.h vector/Vector3.h grid/grid.h game_map/game_map.h main/globals.h

game_map_cpp = game_map/draw.cpp game_map/update.cpp 

object_bot = health_pack/health_pack.cpp bots/bots.cpp bullet/bullet.cpp player/player.cpp weapons/weapons.cpp grid/grid.cpp main/globals.cpp vector/Vector3.cpp game_map/game_map.cpp
object_bot2 = health_pack/health_pack.h bots/bots.h bullet/bullet.h player/player.h weapons/weapons.h grid/grid.h main/globals.h vector/Vector3.h game_map/game_map.h

object_grid1 = player/player.h bullet/bullet.h health_pack/health_pack.h vector/Vector3.h 
object_grid2 = player/player.cpp bullet/bullet.cpp health_pack/health_pack.cpp vector/Vector3.cpp

object_globals = main/globals.h grid/grid.h weapons/weapons.h bullet/bullet.h game_map/game_map.h
object_globals2 = main/globals.cpp grid/grid.cpp weapons/weapons.cpp bullet/bullet.cpp game_map/game_map.cpp

object_extras = extras/directions.h extras/nodes.h extras/field_view.h
object_extras2 = extras/directions.cpp extras/nodes.cpp extras/field_view.cpp

CXX = g++ -g -std=c++11 -w
DRAWFLAGS = -lGL -lGLU -lglut -lsfml-audio
THREADFLAGS = -lpthread
DEBUG = -DTO_DEBUG

C_ALL = $(CXX) $(DRAWFLAGS) $(THREADFLAGS)

all: graphics/GraphicsMain.cpp bots.o Vector3.o extras.o health_pack.o globals.o grid.o player.o game_map.o bullet.o spark.o camera.o lodepng.o image.o
	g++ -o Game graphics/GraphicsMain.cpp Vector3.o directions.o field_view.o health_pack.o globals.o grid.o player.o game_map.o bots.o bullet.o spark.o camera.o lodepng.o image.o nodes.o weapons.o -lGL -lGLU -lglut -lsfml-audio

bots.o: $(object_bot) $(object_bot2) $(object_extras) $(object_extras2)
	$(CXX) -c $(object_bot)	$(object_extras2)
extras.o: $(object_extras) $(object_extras2)
	$(CXX) -c $(object_extras2)

health_pack.o: health_pack/health_pack.h health_pack/health_pack.cpp
	$(CXX) -c health_pack/health_pack.cpp	
Vector3.o: vector/Vector3.h vector/Vector3.cpp
	$(CXX) -c vector/Vector3.cpp
grid.o: grid/grid.h grid/grid.cpp $(object_grid1) $(object_grid2)
	$(CXX) -c grid/grid.cpp $(object_grid2)
bullet.o: bullet/bullet.h bullet/bullet.cpp
	$(CXX) -c bullet/bullet.cpp
game_map.o:  $(game_map_cpp) $(object_bot) $(object_bot2) 
	$(CXX) $(DRAWFLAGS) -c $(game_map_cpp) $(object_bot)
player.o: $(object_bot) $(object_bot2)
	$(CXX) -c $(object_bot)
globals.o: $(object_globals) $(object_globals2)
	$(CXX) $(DRAWFLAGS) -c $(object_globals2)
spark.o: vector/Vector3.cpp vector/Vector3.h graphics/spark.cpp graphics/spark.h
	$(C_ALL) -c vector/Vector3.cpp graphics/spark.cpp
camera.o: graphics/camera.cpp graphics/camera.h
	$(C_ALL) -c graphics/camera.cpp
lodepng.o: graphics/lodepng.cpp graphics/lodepng.h
	$(C_ALL) -c graphics/lodepng.cpp
image.o: graphics/image.cpp graphics/image.h
	$(C_ALL) -c graphics/image.cpp
# network.o: network/network.h network/network.cpp game_map.o globals.o
# 	$(C_ALL) -c network/network.cpp
# thread_functions.o: main/thread_functions.h main/thread_functions.cpp network.o
# 	$(C_ALL) -c main/thread_functions.cpp


clean:
	rm -f *.o Game

join: all
	./Game join $(PEER)

host: all
	./Game host

debug:
	