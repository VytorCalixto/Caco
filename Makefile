CXXFLAGS= -w -g -Wall -O2 -std=c++11
CC=g++

OBJ=Protocol.o Message.o

all: cacoclient cacoserver

%.o: %.cpp
	$(CC) $(CXXFLAGS) $^ -c -o $@
cacoclient: client.cpp $(OBJ)
	$(CC) $(CXXFLAGS) $^ -o $@
cacoserver: server.cpp $(OBJ)
	$(CC) $(CXXFLAGS) $^ -o $@
clean:
	rm -f *.o caco*
