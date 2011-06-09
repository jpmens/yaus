CFLAGS=-Itinycdb-0.77 -Wall
LDFLAGS=-Ltinycdb-0.77 
DEST=../yaus.cgi

all: yaus

yaus: yaus.c
	$(CC) $(CFLAGS) -o yaus yaus.c $(LDFLAGS) -lcdb

install: yaus
	install -m 555 yaus $(DEST)

data: data.cdb

data.cdb: example.in
	cdb -cm -t data.temp data.cdb < example.in
