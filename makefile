default:
	gcc -lm src/main.c src/util/dllist.c -o main 

debug:
	gcc --debug -lm src/main.c src/util/dllist.c -o main 

