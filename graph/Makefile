CFLAGS=`pkg-config libgvc --cflags` -Wall -g -O2
LDLIBS=`pkg-config libgvc --libs`

.PHONY: all
all: main

main: main.o

.PHONY: clean
clean:
	rm -rf *.o main
