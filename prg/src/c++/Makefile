CC=g++
FLAGS=-pthread -std=c++11

all: main

main: math.o funs.o states.o main.o
	$(CC) $(FLAGS) -o main math.o states.o funs.o main.o

perf: math.o funs.o states.o perf.o
	$(CC) $(FLAGS) -o perf math.o states.o funs.o perf.o

%.o: %.c
	$(CC) $(FLAGS) -c $^

%.o: %.cpp
	$(CC) $(FLAGS) -c $^

clemacs:
	rm *~

clean:
	rm *.o

mrproper:
	rm *.o
	rm main


