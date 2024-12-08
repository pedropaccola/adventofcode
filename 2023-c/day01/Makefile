EXAMPLE_FILE = "ex.txt"
INPUT_FILE = "in.txt"

build1: 
	@gcc -o 1.out 1.c

build2:
	@gcc -o 2.out 2.c

test1: build1
	@echo "Example part 1:"
	@./1.out $(EXAMPLE_FILE)

final1: build1
	@echo "Final input part 1:"
	@./1.out $(INPUT_FILE)

test2: build2
	@echo "Example part 2:"
	@./2.out $(EXAMPLE_FILE)

final2: build2
	@echo "Final input part 2:"
	@./2.out $(INPUT_FILE)
