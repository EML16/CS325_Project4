CC=g++
CFLAGS = -g -std=c++11 -I.
LFLAGS = ""
DEPS = Graph.h

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


Driver: Graph.o Driver.o  
	$(CC)  -o Driver Graph.o Driver.o
clean:
	rm *.o
