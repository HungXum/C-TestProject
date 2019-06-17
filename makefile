start:
	g++ -g -c main.cpp
	g++ main.o -o program
clean:
	rm *.o program
