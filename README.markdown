# YAUS

`yaus` is Yet Another URL Shortner. It uses a CDB database from which it
reads keys and URLs. When installed as a CGI program, it will redirect
a key to the pertaining URL.

### Data

The `Makefile` has an example on how to build the `data.cdb` file. Enter
either one of the following commands to build `data.cdb` from the
provided `example.in`.

	make data
	cdb -cm -t data.temp data.cdb < example.in

`example.in` is a list of white-space separated key/URLs.

	1	http://google.com
	jp	http://jpmens.org
	ip	http://jpmens.net/2010/02/15/the-coolest-ip-on-the-internet/

To check the content of the database, use TinyCDB's `cdb`  utility as

	cdb -d data.cdb 
	+1,17:1->http://google.com
	+2,17:jp->http://jpmens.org
	+2,60:ip->http://jpmens.net/2010/02/15/the-coolest-ip-on-the-internet/

### .htaccess

You'll need to set up `yaus` as a CGI program. Used on Apache, the following
`.htaccess` should point you in the right direction:

	Options +ExecCGI +FollowSymLinks -Indexes

	AddHandler cgi-script .cgi
	DirectoryIndex yaus.cgi

	RewriteEngine on
	RewriteBase /redir
	RewriteCond     %{REQUEST_FILENAME}     !-f
	RewriteCond     %{REQUEST_FILENAME}     !-d
	RewriteRule ^(.*)$      yaus.cgi?$1     [PT,QSA]

This assumes your installation of `yaus` is located at `/redir`; if it isn't
you'll have to tweak your `RewriteBase`.

### TinyCDB

[TinyCDB](http://www.corpit.ru/mjt/tinycdb.html) is included here for convenience.
It was written by Michael Tokarev, and its terms of usage are:

> The code is in public domain, that is, you may do anything you want with it.

### Bugs

Bugs? Sure, how many do you want? I can think of these:

* No loop detection. (Don't add a URL that points to `yaus`...)
* Keys with a plus (`+`) in them are chomped at the plus. That's a feature.
