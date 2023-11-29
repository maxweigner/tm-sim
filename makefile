default:
	gcc -lm src/tm_sim.cpp src/main.cpp src/util/dllist.cpp -o main

debug:
	gcc --debug -lm src/main.cpp src/tm_sim.cpp src/util/dllist.cpp -o main

