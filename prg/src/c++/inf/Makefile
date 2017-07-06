CC=g++
FLAGS=-pthread -std=c++11

all: main

main.o: main.cpp
	$(CC) $(FLAGS) -c main.cpp

%.o: ../%.c
	$(CC) $(FLAGS) -c $^

%.o: ../%.cpp
	$(CC) $(FLAGS) -c $^

main: math.o funs.o states.o main.o
	$(CC) $(FLAGS) -o main math.o states.o funs.o main.o

clemacs:
	rm *~

clean:
	rm *.o

mrproper:
	rm *.o
	rm main
