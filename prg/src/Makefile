all:
	ocamlbuild main.native
clean:
	ocamlbuild -clean
clemacs:
	rm *~
tool: toolkit.ml
	ocamlbuild toolkit.native
simu:
	ocamlbuild -pkg unix simu.native
