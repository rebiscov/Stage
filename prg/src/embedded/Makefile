CC=g++
FLAGS=-std=c++11 -pthread

all: client serveur

client: math.o states.o client.o
	$(CC) $(FLAGS) -o client math.o states.o client.o

server: math.o states.o server.o
	$(CC) $(FLAGS) -o server math.o states.o server.o

%.o: %.c
	$(CC) $(FLAGS) -c %.c

clean:
	rm *.o

mrproper: clean
	rm client
	rm server



