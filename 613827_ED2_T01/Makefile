.PHONY: all clean

all: main

main: main.o 613827_ED2_T01.o
	gcc -o main 613827_ED2_T01.o main.o

main.o:
	gcc -c main.c

613827_ED2_T01.o:
	gcc -c 613827_ED2_T01.c

clean:
	rm -f 613827_ED2_T01.o
	rm -f main.o
	rm -f main
