CC=cc
CFLAGS=-g -Wall -std=c99 -pedantic
CFLG_GLIB=$(shell pkg-config --cflags glib-2.0)

default: clean fdt_client

clean:
	rm -rf bin
	mkdir bin

gui:
	$(CC) -c -o bin/gui.o src/gui/gui.c -lncurses -lglib-2.0 $(CFLAGS) $(CFLG_GLIB)
	$(CC) -c -o bin/gui_tabs.o src/gui/gui_tabs.c -lncurses $(CFLAGS)
	$(CC) -c -o bin/gui_panel.o src/gui/gui_panel.c -lncurses $(CFLAGS)

parsers:
	$(CC) -c -o bin/p_panel.o src/parsers/p_panel.c -lglib-2.0 $(CFLAGS) $(CFLG_GLIB)

net:
	$(CC) -c -o bin/http_client.o src/net/http_client.c -lglib-2.0 -lcurl $(CFLAGS) $(CFLG_GLIB)

handlers:
	$(CC) -c -o bin/h_panel.o src/handlers/h_panel.c $(CFLAGS) $(CFLG_GLIB)

main:
	$(CC) bin/*.o -o bin/fdt_client src/main.c -lncurses -lglib-2.0 -lcurl $(CFLAGS) $(CFLG_GLIB)

fdt_client: gui parsers net handlers main

