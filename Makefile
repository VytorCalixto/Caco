CFLAGS= -w -g -Wall -O2
CC=g++

OBJ=Protocol.o

%.o: %.cpp
	$(CC) $(CFLAGS) $^ -c -o $@
cacoclient: client.cpp $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@
cacoserver: $(OBJ) server.cpp
	$(CC) $(CFLAGS) $^ -o $@
clean:
	rm -f *.o caco*
