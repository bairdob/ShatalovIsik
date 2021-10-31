all:  main

main: main.o 
	g++ -std=c++17 main.o -o main 

main.o: main.cpp
	g++ -std=c++17 -c main.cpp 

clean:
	rm -rf *.o  main