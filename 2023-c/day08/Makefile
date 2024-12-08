CC = gcc
CFLAGS = -Wall -Wextra -std=c17 -pedantic -ggdb

EXAMPLE_FILE = "ex.txt"
INPUT_FILE = "in.txt"

one: one.c 
	@echo "Example part 1:"
	@$(CC) $(CFLAGS) -o one.o one.c && ./one.o $(EXAMPLE_FILE)

two: two.c
	@echo "Example part 2:"
	$(CC) $(CFLAGS) -o two.o two.c && ./two.o $(EXAMPLE_FILE)

onef: one.c 
	@echo "Final input part 1:"
	$(CC) $(CFLAGS) -o one.o one.c && ./one.o $(INPUT_FILE)

twof: two.c
	@echo "Final input part 2:"
	$(CC) $(CFLAGS) -o two.o two.c && ./two.o $(INPUT_FILE)

clean:
	rm -f *.o
