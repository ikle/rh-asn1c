TARGETS  = mscode-asn1.o pkcs7-asn1.o rsaprivkey-asn1.o rsapubkey-asn1.o
TARGETS	+= x509_akid-asn1.o x509-asn1.o x509_rsakey-asn1.o
TARGETS += x509-show

CFLAGS	+= -I ../include
ASN1C	?= ../rh-asn1c

all: $(TARGETS)

clean:
	rm -f *.o *-asn1.[ch] $(TARGETS)

PREFIX ?= /usr/local

install: $(TARGETS)
	install -D -d $(DESTDIR)/$(PREFIX)/bin
	install -s -m 0755 $^ $(DESTDIR)/$(PREFIX)/bin

%-asn1.o: %-asn1.c %-asn1.h
%-asn1.c %-asn1.h: %.asn1
	$(ASN1C) $^

x509-show: x509-asn1.o ../librh-asn1c.a
