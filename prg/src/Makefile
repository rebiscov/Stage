all:
	ocamlbuild main.native
clean:
	ocamlbuild -clean
clemacs:
	rm *~
tool: toolkit.ml
	ocamlbuild toolkit.native
simu: simu.ml
	ocamlbuild -pkg unix simu.native
