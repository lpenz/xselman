
PREFIX=/usr/local
PLUGINDIR = $(PREFIX)/lib/gkrellm2/plugins

GTK2_INCLUDE = `pkg-config gtk+-2.0 --cflags`
GTK2_LIB = `pkg-config gtk+-2.0 --libs`

CPPFLAGS = -Wall -Werror
CFLAGS = -O2 -fPIC $(GTK2_INCLUDE)
LDLIBS = $(GTK2_LIB)

all: xselman

.PHONY: all install clean

xselman: xselman.c


#install: gkrellexec.so
#	mkdir -p $(PLUGINDIR)
#	install -c -s -m 644 gkrellexec.so $(PLUGINDIR)

clean:
	rm -f *.o *.so

#README.textile: gkrellexec.t2t
#	txt2tags -t html -H -i $^ -o $@
#	sed -i -e 's@<B>@**@g' -e 's@</B>@**@g' $@
#
#README.html: gkrellexec.t2t
#	txt2tags -t html -i $< -o $@

