CC = ocamlopt
CCH = ocamlc

all: main.native

math.cmx: math.ml
	$(CC) -c math.ml

states.cmx: states.ml states.cmi
	$(CC) -c states.ml

states.cmi: states.mli
	$(CCH) -c states.mli

main.cmx: main.ml
	$(CC) -c main.ml

funs.cmx: funs.ml
	$(CC) -c funs.ml

fun_c.o: fun_c.c
	$(CC) -c fun_c.c

funs.o: c++/funs.c
	gcc -c c++/funs.c

perf.cmx: perf.ml
	$(CC) -c perf.ml

toolkit.cmx: toolkit.ml
	$(CC) -c toolkit.ml

simu.cmx: simu.ml
	$(CC) -c simu.ml

main.native: math.cmx fun_c.o funs.cmx states.cmx main.cmx
	$(CC) math.cmx fun_c.o funs.cmx states.cmx main.cmx -o main.native
	make clean

clean:
	rm *.cmi *.cmx *.o

clemacs:
	rm *~

tool: tool.native

tool.native: math.cmx fun_c.o funs.cmx states.cmx toolkit.cmx
	$(CC) math.cmx fun_c.o funs.cmx states.cmx toolkit.cmx -o toolkit.native
	make clean

simu: simu.native

simu.native: math.cmx fun_c.o funs.cmx states.cmx simu.cmx
	$(CC) unix.cmxa math.cmx fun_c.o funs.cmx states.cmx simu.cmx -o simu.native
	make clean

perf: perf.native

perf.native: math.cmx fun_c.o funs.cmx states.cmx perf.cmx
	$(CC) math.cmx fun_c.o funs.cmx states.cmx perf.cmx -o perf.native
	make clean

simulation:
	./proba/proba $(d) $(s) distri.txt
	make -C c++ main
	./c++/main $(t) $(v_max) distri.txt pol.txt d
	make -C c++ clean
	make simu
	./simu.native $(t) $(v_max) distri.txt pol.txt simu.txt $(nb_simu)
	rm simu.native
	mv simu.txt simu

compare:
	./proba/proba $(d) $(s) distri.txt
	make -C c++ main
	./c++/main $(t) $(v_max) distri.txt pol1.txt d
	./c++/main $(t) $(v_max) distri.txt pol2.txt d no_ov
	make -C c++ clean
	make simu
	./simu.native $(t) $(v_max) distri.txt pol1.txt simu1.txt $(nb_simu) $(seed)
	./simu.native $(t) $(v_max) distri.txt pol2.txt simu2.txt $(nb_simu) $(seed) no_ov
	rm simu.native
	mv simu1.txt simu
	mv simu2.txt simu

