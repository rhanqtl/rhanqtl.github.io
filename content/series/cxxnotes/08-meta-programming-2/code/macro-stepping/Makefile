.PHONY: gdb

gdb:
	gcc -gdwarf-2 -g3 main.c
	gdb -nw a.out -ex 'start' -ex 'macro expand-once OUTER(xyz)'
