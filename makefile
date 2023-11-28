default:
	gcc -lm src/tm-sim.c src/util/dllist.c -o main 

debug:
	gcc --debug -lm src/tm-sim.c src/util/dllist.c -o main 

