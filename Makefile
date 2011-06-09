CFLAGS=-Itinycdb-0.77 -Wall
LDFLAGS=-Ltinycdb-0.77 

all: yaus.cgi

yaus.cgi: yaus.c
	$(CC) $(CFLAGS) -o yaus.cgi yaus.c $(LDFLAGS) -lcdb
