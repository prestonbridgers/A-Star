default: grid.o
	gcc grid.o main.c -L. -la_star
grid.o: grid.c

lib: a_star.o
	ar -rcv liba_star.a a_star.o
a_star.o: a_star.c

clean:
	rm *.out *.o
