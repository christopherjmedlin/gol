objects = gol.o ui.o

libraries = -lncurses

CFLAGS=-DDEBUG
CC=gcc -g3 $(CFLAGS)

gol : $(objects) main.o
	$(CC) $(libraries) -o gol $(objects) main.o

$(objects) tests.o main.o : %.o: %.c
	$(CC) $(libraries) -c $< -o $@

.PHONY : clean
clean :
	-rm gol tests $(objects) main.o tests.o

tests : $(objects) tests.o
	$(CC) $(libraries) -o tests $(objects) tests.o

test : tests
	valgrind ./tests
