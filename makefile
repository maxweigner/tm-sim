default:
	gcc -lm src/tm_sim.cpp src/util/dllist.c -o main 

debug:
	gcc --debug -lm src/tm_sim.cpp src/util/dllist.c -o main 

