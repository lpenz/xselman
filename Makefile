
PREFIX=/usr/local
BINDIR=$(PREFIX)/bin
MANDIR=$(PREFIX)/share/man

GTK2_INCLUDE = `pkg-config gtk+-2.0 --cflags`
GTK2_LIB = `pkg-config gtk+-2.0 --libs`

CFLAGS = -Wall -Werror -O2 $(GTK2_INCLUDE)
LDLIBS = $(GTK2_LIB)


all: xselman xselman.1

.PHONY: all install clean

xselman: xselman.o

xselman.o: xselman.c

xselman.1: manual.t2t
	txt2tags -t man -i $^ -o $@

README.textile: manual.t2t
	txt2tags -t html -H -i $^ -o $@
	sed -i -e 's@<B>@**@g' -e 's@</B>@**@g' $@

clean:
	rm -f xselman.o xselman

install: xselman
	install -d $(BINDIR) $(MANDIR)/man1
	install xselman $(BINDIR)/xselman
	install xselman.1 $(MANDIR)/man1/xselman.1

