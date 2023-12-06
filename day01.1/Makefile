EXAMPLE_FILE = "ex.txt"
INPUT_FILE = "in.txt"

build: 
	@gcc -o code.out code.c

test: build
	@echo "Example:"
	@./code.out $(EXAMPLE_FILE)

final: build
	@echo "Input:"
	@./code.out $(INPUT_FILE)
