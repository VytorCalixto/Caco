CFLAGS= -w -g -Wall -O2
CC=g++

OBJ=Protocol.o Message.o

all: cacoclient cacoserver

%.o: %.cpp
	$(CC) $(CFLAGS) $^ -c -o $@
cacoclient: client.cpp $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@
cacoserver: server.cpp $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@
clean:
	rm -f *.o caco*
