ASN1C    = rh-asn1c
LIBRARY  = librh-asn1c.a

TARGETS  = $(ASN1C) $(LIBRARY)

CFLAGS	+= -I$(CURDIR)/include

.PHONY: all clean install examples

all: $(TARGETS)

RANLIB ?= ranlib

$(LIBRARY): asn1-decoder.o
	$(AR) rc $@ $^
	$(RANLIB) $@

clean:
	rm -f *.o $(TARGETS)
	make -C examples clean

PREFIX ?= /usr/local

install: $(TARGETS)
	install -D -d $(DESTDIR)/$(PREFIX)/bin
	install -s -m 0755 $^ $(DESTDIR)/$(PREFIX)/bin

	install -D -d $(DESTDIR)/$(PREFIX)/include/rh-asn1c
	cp include/rh-asn1c/*.h $(DESTDIR)/$(PREFIX)/include/rh-asn1c

	install -D -d $(DESTDIR)/$(PREFIX)/lib
	install -m 0644 $(LIBRARY) $(DESTDIR)/$(PREFIX)/lib

examples: $(TARGETS)
	make -C $@
