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

simulation:
	./proba/proba $(d) $(s) distri.txt
	make -C c++ main
	./c++/main $(t) $(v_max) distri.txt pol.txt d
	make -C c++ clean
	make simu
	./simu.native $(t) $(v_max) distri.txt pol.txt simu.txt $(nb_simu)
	make clean
	mv simu.txt simu

