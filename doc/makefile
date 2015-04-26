server_files_cpp = server_functions.cpp thread_function.cpp server_main.cpp password.cpp
server_files_h = server_functions.h thread_function.h password.h share_rollback_server.h
server_database_h = server_log_database.h server_file_database.h
server_database_cpp = server_log_database.cpp server_file_database.cpp
classes_cpp = doublet.cpp quadlet.cpp cts.cpp stc.cpp
classes_h = doublet.h quadlet.h cts.h stc.h

all: common_functions.o cts.o globals.o password.o quadlet.o server_file_database.o server_functions.o server_log_database.o server_main.o share_rollback_server.o stc.o thread_function.o
	g++ -std=c++11 -w -Wall -o server common_functions.o cts.o globals.o password.o quadlet.o server_file_database.o server_functions.o server_log_database.o server_main.o share_rollback_server.o stc.o thread_function.o -L/usr/lib -lssl -lcrypto -lpthread  -lsqlite3



common_functions.o: $(common_files)
	g++ -std=c++11 -w -Wall -c common_functions.cpp -L/usr/lib -lssl -lcrypto -lpthread -lsqlite3 

cts.o: cts.cpp cts.h
	g++ -std=c++11 -w -Wall -c cts.cpp -lsqlite3 

globals.o: globals.cpp globals.h
	g++ -std=c++11 -w -Wall -c globals.cpp -L/usr/lib -lssl -lcrypto -lpthread -lsqlite3 

password.o: password.cpp password.h
	g++ -std=c++11 -w -Wall -c password.cpp -L/usr/lib -lssl -lcrypto -lpthread -lsqlite3 

quadlet.o: quadlet.cpp quadlet.h
	g++ -std=c++11 -w -Wall -c quadlet.cpp -lsqlite3 

server_file_database.o: $(server_database_cpp) $(classes_cpp) server_functions.cpp common_functions.cpp $(server_database_h) $(classes_h) server_functions.h common_functions.h
	g++ -std=c++11 -w -Wall -c $(server_database_cpp) $(classes_cpp) server_functions.cpp common_functions.cpp -L/usr/lib -lssl -lcrypto -lpthread -lsqlite3 

server_functions.o: server_functions.cpp common_functions.cpp server_log_database.cpp password.cpp stc.cpp cts.cpp globals.cpp share_rollback_server.cpp server_functions.h common_functions.h server_log_database.h password.h stc.h cts.h globals.h share_rollback_server.h
	g++ -std=c++11 -w -Wall -c server_functions.cpp common_functions.cpp server_log_database.cpp password.cpp stc.cpp cts.cpp globals.cpp share_rollback_server.cpp -L/usr/lib -lssl -lcrypto -lpthread -lsqlite3 
 
server_log_database.o: $(server_database_cpp) cts.cpp stc.cpp $(server_database_h) cts.h stc.h
	g++ -std=c++11 -w -Wall -c  $(server_database_cpp) cts.cpp stc.cpp -lsqlite3 

server_main.o: $(server_files_cpp) $(server_database_cpp) common_functions.cpp globals.cpp  $(server_files_h) $(server_database_h) common_functions.h globals.h
	g++ -std=c++11 -w -Wall -c $(server_files_cpp) $(server_database_cpp) common_functions.cpp globals.cpp -L/usr/lib -lssl -lcrypto -lpthread -lsqlite3 

share_rollback_server.o: share_rollback_server.cpp $(server_database_cpp) share_rollback_server.h $(server_database_h)
	g++ -std=c++11 -w -Wall -c share_rollback_server.cpp $(server_database_cpp) -L/usr/lib -lssl -lcrypto -lpthread -lsqlite3 

stc.o: stc.cpp stc.h
	g++ -std=c++11 -w -Wall -c stc.cpp -lsqlite3 

thread_function.o: thread_function.cpp server_functions.cpp common_functions.cpp password.cpp thread_function.h server_functions.h common_functions.h password.h
	g++ -std=c++11 -w -Wall -c thread_function.cpp server_functions.cpp common_functions.cpp password.cpp -L/usr/lib -lssl -lcrypto -lpthread -lsqlite3 



common_functions_debug.o: $(common_files)
	g++  -DTO_DEBUG -std=c++11 -w -Wall -c common_functions.cpp -L/usr/lib -lssl -lcrypto -lpthread -lsqlite3 

cts_debug.o: cts.cpp cts.h
	g++  -DTO_DEBUG -std=c++11 -w -Wall -c cts.cpp -lsqlite3 

globals_debug.o: globals.cpp globals.h
	g++  -DTO_DEBUG -std=c++11 -w -Wall -c globals.cpp -L/usr/lib -lssl -lcrypto -lpthread -lsqlite3 

password_debug.o: password.cpp password.h
	g++  -DTO_DEBUG -std=c++11 -w -Wall -c password.cpp -L/usr/lib -lssl -lcrypto -lpthread -lsqlite3 

quadlet_debug.o: quadlet.cpp quadlet.h
	g++  -DTO_DEBUG -std=c++11 -w -Wall -c quadlet.cpp -lsqlite3 

server_file_database_debug.o: $(server_database_cpp) $(classes_cpp) server_functions.cpp common_functions.cpp $(server_database_h) $(classes_h) server_functions.h common_functions.h
	g++  -DTO_DEBUG -std=c++11 -w -Wall -c $(server_database_cpp) $(classes_cpp) server_functions.cpp common_functions.cpp -L/usr/lib -lssl -lcrypto -lpthread -lsqlite3 

server_functions_debug.o: server_functions.cpp common_functions.cpp server_log_database.cpp password.cpp stc.cpp cts.cpp globals.cpp share_rollback_server.cpp server_functions.h common_functions.h server_log_database.h password.h stc.h cts.h globals.h share_rollback_server.h
	g++  -DTO_DEBUG -std=c++11 -w -Wall -c server_functions.cpp common_functions.cpp server_log_database.cpp password.cpp stc.cpp cts.cpp globals.cpp share_rollback_server.cpp -L/usr/lib -lssl -lcrypto -lpthread -lsqlite3 
 
server_log_database_debug.o: $(server_database_cpp) cts.cpp stc.cpp $(server_database_h) cts.h stc.h
	g++  -DTO_DEBUG -std=c++11 -w -Wall -c  $(server_database_cpp) cts.cpp stc.cpp -lsqlite3 

server_main_debug.o: $(server_files_cpp) $(server_database_cpp) common_functions.cpp globals.cpp  $(server_files_h) $(server_database_h) common_functions.h globals.h
	g++  -DTO_DEBUG -std=c++11 -w -Wall -c $(server_files_cpp) $(server_database_cpp) common_functions.cpp globals.cpp -L/usr/lib -lssl -lcrypto -lpthread -lsqlite3 

share_rollback_server_debug.o: share_rollback_server.cpp $(server_database_cpp) share_rollback_server.h $(server_database_h)
	g++  -DTO_DEBUG -std=c++11 -w -Wall -c share_rollback_server.cpp $(server_database_cpp) -L/usr/lib -lssl -lcrypto -lpthread -lsqlite3 

stc_debug.o: stc.cpp stc.h
	g++  -DTO_DEBUG -std=c++11 -w -Wall -c stc.cpp -lsqlite3 

thread_function_debug.o: thread_function.cpp server_functions.cpp common_functions.cpp password.cpp thread_function.h server_functions.h common_functions.h password.h
	g++ -DTO_DEBUG -std=c++11 -w -Wall -c thread_function.cpp server_functions.cpp common_functions.cpp password.cpp -L/usr/lib -lssl -lcrypto -lpthread -lsqlite3 


clean:
	rm *o all; rm server

start_server: all
	sudo ./server $(PORT)

debug: common_functions_debug.o cts_debug.o globals_debug.o password_debug.o quadlet_debug.o server_file_database_debug.o server_functions_debug.o server_log_database_debug.o server_main_debug.o share_rollback_server_debug.o stc_debug.o thread_function_debug.o
	g++ -DTO_DEBUG -std=c++11 -w -Wall -o server common_functions.o cts.o globals.o password.o quadlet.o server_file_database.o server_functions.o server_log_database.o server_main.o share_rollback_server.o stc.o thread_function.o -L/usr/lib -lssl -lcrypto -lpthread  -lsqlite3; \
	sudo ./server $(PORT)

