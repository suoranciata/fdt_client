SHELL=/bin/zsh
CC=cc
CFLAGS=-std=c99 -pedantic -Wall -g -Isrc

OBJS=bin/http/client.o bin/parsers/panel.o

default: clean build

clean:
	rm -rf bin
	mkdir bin
	mkdir -p bin/{util,http,parsers}

build:
	$(CC) -o bin/http/client.o src/http/client.c -c $(CFLAGS) -lcurl
	$(CC) -o bin/parsers/panel.o src/parsers/panel.c -c $(CFLAGS)
	$(CC) $(OBJS) -o bin/main src/main.c $(CFLAGS) -lpcre -lcurl

test: clean build
	$(CC) $(OBJS) -o bin/test src/test.c $(CFLAGS) -lpcre -lcurl

run-test: test
	bin/test

dbg-test: test
	valgrind bin/test

