#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <cdb.h>

#define DB	"data.cdb"

static char *lookup(struct cdb *cdb, char *);

int main(int argc, char **argv, char **envp)
{
        struct cdb cdb;
	int fd, show = 0;
	char *key, keybuf[BUFSIZ], *p, *url;

        if (((fd = open(DB, O_RDONLY)) == -1) ||
			(cdb_init(&cdb, fd) != 0)) {
		printf("Content-type: text/plain\n\n");
		printf("Can't open database: %s\n", strerror(errno));
		return (0);
	}

	key = (argc == 2) ? argv[1] : getenv("QUERY_STRING");
	if (!key || !*key) {
		printf("Content-type: text/plain\n\n");
		printf("Huh?\n");
		return (0);
	}


	strncpy(keybuf, key, sizeof(keybuf) - 1);
	keybuf[BUFSIZ - 1] = 0;

	if (strlen(keybuf) > 1) {
		if ((p = strrchr(keybuf, '+')) != NULL) {
			show = 1;
			*p = 0;
		}
	}

	url = lookup(&cdb, keybuf);
	if (url == NULL) {
		printf("Content-type: text/plain\n\n");
		printf("Key not found\n");
	} else {
		if (show) {
			printf("Content-type: text/html\n\n");
			printf("<a href='%s'>%s</a>\n", url, url);
		} else {
			printf("Status: 301\n");
			printf("X-redirector: yaus\n");
			printf("Location: %s\n", url);
			printf("Content-length: 0\n");
			printf("Content-type: text/plain\n\n");
		}
	}

	cdb_free(&cdb);
	close(fd);
	return 0;
}

static char *lookup(struct cdb *cdb, char *key)
{
        char *data;
        unsigned keylen, datalen;

#if TESTING
	unsigned k;

	if (!key || !*key) {
		cdb_seqinit(&k, cdb);

		while (cdb_seqnext(&k, cdb) > 0) {
			keylen = cdb_keylen(cdb);
			key = malloc(keylen + 1);
			cdb_read(cdb, key, keylen, cdb_keypos(cdb));
			key[keylen] = '\0';

			datalen = cdb_datalen(cdb);
			data = malloc(datalen + 1);
			cdb_read(cdb, data, datalen, cdb_datapos(cdb));
			data[datalen] = '\0';

			printf("key=%s data=%s\n", key, data);
			free(key);
			free(data);
		}
		return 0;
	}
#endif /* TESTING */
        
	keylen = strlen(key);

	if (cdb_find(cdb, key, keylen) <= 0) {
		return (NULL);
	}

	datalen = cdb_datalen(cdb);
	data = malloc(datalen + 1);
	cdb_read(cdb, data, datalen, cdb_datapos(cdb));
	data[datalen] = '\0';

	return (data);
}
