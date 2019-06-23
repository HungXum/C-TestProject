start:
	g++ -g -c -std=c++11 main.cpp
	g++ main.o -o program
clean:
	rm *.o program
