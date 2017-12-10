all: control.o main.o
	gcc -o main.out main.o
	gcc -o control.out control.o


main.o: main.c shared_header.h
	gcc -c main.c

control.o: control.c shared_header.h
	gcc -c control.c

clean:
	./control.out -r || true
	rm -f *.o
	rm -f *.out
	rm -f story.txt
