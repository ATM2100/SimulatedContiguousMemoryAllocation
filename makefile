FILE = SimulatedMemoryAllocation

all: $(FILE).c
	gcc -c -g $(FILE).c -o $(FILE).o
	gcc $(FILE).o -o $(FILE)
