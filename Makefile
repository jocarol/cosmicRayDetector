all: bitflip

bitflip: scanner.c
	$(CC) -O3 -Wall -W -pedantic -std=c99 -o scanner scanner.c

clean:
	rm scanner
