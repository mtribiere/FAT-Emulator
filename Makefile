build:
	gcc -Wall -o prog main.c fat.h fat.c

debug:
	gcc -Wall -g -o prog main.c fat.h fat.c

run:
	./prog

clean:
	rm *.o
