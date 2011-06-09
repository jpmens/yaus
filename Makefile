CFLAGS=-Itinycdb-0.77 -Wall
LDFLAGS=-Ltinycdb-0.77 
DEST=../yaus.cgi

all: yaus

yaus: yaus.c
	$(CC) $(CFLAGS) -o yaus yaus.c $(LDFLAGS) -lcdb

install: yaus
	install -m 555 yaus $(DEST)
