CC = ocamlbuild

all: main.native

main.native: main.ml
	$(CC) main.native

clean:
	$(CC) -clean

clemacs:
	rm *~

tool: tool.native

tool.native: toolkit.ml
	$(CC) toolkit.native

simu: simu.native

simu.native: simu.ml
	$(CC) -pkg unix simu.native

perf: perf.native

perf.native: perf.ml
	$(CC) perf.native
