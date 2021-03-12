libs = -ljansson

all: IPrelease
 
IPrelease: IPrelease.o
	cc -g -o IPrelease IPrelease.o -ljansson


decode.o: decode.c
	cc -c -Wall -g decode.c

IPrelease.o: IPrelease.c decode.h decode.c
	cc -c -Wall -g IPrelease.c
	cc -c -Wall -g decode.h 
	cc -c -Wall -g decode.c
clean:
	rm IPrelease IPrelease.o decode.o

