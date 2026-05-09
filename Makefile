check: spell-checker.o trie.o interactive.o
	gcc -o check spell-checker.o trie.o interactive.o
	rm -f *.o

spell-checker.o: spell-checker.c trie.h interactive.h
	gcc -c spell-checker.c

trie.o: trie.c trie.h
	gcc -c trie.c

interactive.o: interactive.c interactive.h
	gcc -c interactive.c

clean:
	rm -f *.o check