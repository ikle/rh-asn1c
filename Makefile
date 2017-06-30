TARGETS  = rh-asn1c asn1-decoder.o

CFLAGS	+= -I$(CURDIR)/include

all: $(TARGETS)

clean:
	rm -f *.o $(TARGETS)

PREFIX ?= /usr/local

install: $(TARGETS)
	install -D -d $(DESTDIR)/$(PREFIX)/bin
	install -s -m 0755 $^ $(DESTDIR)/$(PREFIX)/bin
