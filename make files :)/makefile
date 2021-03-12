libs = -ljansson

all: runner
 
runner: runner.o
	cc -g -o runner runner.o -ljansson


decode.o: decode.c
	cc -c -Wall -g decode.c

runner.o: runner.c decode.h decode.c
	cc -c -Wall -g runner.c
	cc -c -Wall -g decode.h 
	cc -c -Wall -g decode.c
clean:
	rm runner runner.o decode.o

