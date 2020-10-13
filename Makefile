CC = gcc
CFLAGS = -c

LIBFLAGS = -L. -la_star

OBJ = grid.o main.o

.c.o:
	$(CC) $(CFLAGS) $<

pathfinding: $(OBJ) lib
	$(CC) $(OBJ) $(LIBFLAGS) -o $@

main.o: a_star.h grid.h
grid.o: grid.h
a_star.o: a_star.h

lib: a_star.o
	ar -rcv liba_star.a a_star.o

clean:
	rm *.out *.o

install:
	make clean
	make lib
	make

clean_win:
	rm *.o *.exe

install_win:
	make clean_win
	make lib
	make

