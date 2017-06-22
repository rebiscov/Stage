all:
	g++ -c math.cpp states.cpp funs.cpp main.cpp -pthread
	g++ -o main math.o states.o main.o funs.o -pthread

clemacs:
	rm *~

clean:
	rm *.o
mrproper:
	rm *.o
	rm main


