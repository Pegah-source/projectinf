libs = -ljansson

all: IPoverTwitter
 
IPoverTwitter: IPoverTwitter.o
	cc -g -o IPoverTwitter IPoverTwitter.o -ljansson


encode.o: encode.c
	cc -c -Wall -g encode.c

IPoverTwitter.o: IPoverTwitter.c encode.h encode.c
	cc -c -Wall -g IPoverTwitter.c
	cc -c -Wall -g encode.h 
	cc -c -Wall -g encode.c
clean:
	rm IPoverTwitter IPoverTwitter.o encode.o

