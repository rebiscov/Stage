all:
	g++ -std=c++11 -c math.cpp states.cpp funs.cpp main.cpp -pthread
	g++ -std=c++11 -o main math.o states.o main.o funs.o -pthread

perf:
	g++ -std=c++11 -c math.cpp states.cpp funs.cpp perf.cpp -pthread
	g++ -std=c++11 -o perf math.o states.o perf.o funs.o -pthread

clemacs:
	rm *~

clean:
	rm *.o
mrproper:
	rm *.o
	rm main


