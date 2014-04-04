cd ./src/
g++ -I /usr/local/include/boost -Wl,-rpath=/usr/local/lib client.cpp  httpprocessor.cpp  httpprotocol.cpp  httpserver.cpp  main.cpp  socketconnect.cpp -o server -lboost_system -lboost_thread -lssl -lpthread -lboost_regex
