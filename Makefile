TARGETS  = rh-asn1c asn1-decoder.o

CFLAGS	+= -I$(CURDIR)/include

.PHONY: all clean install examples

all: $(TARGETS)

clean:
	rm -f *.o $(TARGETS)
	make -C examples clean

PREFIX ?= /usr/local

install: $(TARGETS)
	install -D -d $(DESTDIR)/$(PREFIX)/bin
	install -s -m 0755 $^ $(DESTDIR)/$(PREFIX)/bin

examples: rh-asn1c
	make -C $@
