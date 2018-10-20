#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include <err.h>

#include "x509-asn1.h"

static int dump(FILE *to, const char *prefix, const char *suffix,
		const void *value, size_t len)
{
	const unsigned char *p;

	fprintf (to, "%s", prefix);

	for (p = value; len > 0; ++p, --len)
		fprintf (to, "%02x", *p);

	fprintf (to, "%s", suffix);
	return 0;
}

static int dump_str(FILE *to, const char *prefix, const char *suffix,
		    const void *value, size_t len)
{
	fprintf (to, "%s%.*s%s",
		 prefix, (int) len, (const char *) value, suffix);
	return 0;
}

int x509_extract_key_data(void *context, size_t hdrlen, unsigned char tag,
			  const void *value, size_t len)
{
	return dump (context, "keydata = ", "\n", value, len);
}

int x509_extract_name_segment(void *context, size_t hdrlen, unsigned char tag,
			      const void *value, size_t len)
{
	return dump_str (context, "name = ", "\n", value, len);
}

int x509_note_OID(void *context, size_t hdrlen, unsigned char tag,
		  const void *value, size_t len)
{
	return dump (context, "oid(", ")\n", value, len);
}

int x509_note_issuer(void *context, size_t hdrlen, unsigned char tag,
		     const void *value, size_t len)
{
	fprintf (context, "got issuer\n");
	return 0;
}

int x509_note_not_after(void *context, size_t hdrlen, unsigned char tag,
			const void *value, size_t len)
{
	return dump_str (context, "from date = ", "\n", value, len);
}

int x509_note_not_before(void *context, size_t hdrlen, unsigned char tag,
			 const void *value, size_t len)
{
	return dump_str (context, "to date = ", "\n", value, len);
}

int x509_note_pkey_algo(void *context, size_t hdrlen, unsigned char tag,
			const void *value, size_t len)
{
	fprintf (context, "got pkey algo\n");
	return 0;
}

int x509_note_serial(void *context, size_t hdrlen, unsigned char tag,
		     const void *value, size_t len)
{
	return dump (context, "serial = ", "\n", value, len);
}

int x509_note_signature(void *context, size_t hdrlen, unsigned char tag,
			const void *value, size_t len)
{
	return dump (context, "signature = ", "\n", value, len);
}

int x509_note_subject(void *context, size_t hdrlen, unsigned char tag,
		      const void *value, size_t len)
{
	fprintf (context, "got subject\n");
	return 0;
}

int x509_note_tbs_certificate(void *context, size_t hdrlen, unsigned char tag,
			      const void *value, size_t len)
{
	fprintf (context, "got certificate body\n");
	return 0;
}

int x509_process_extension(void *context, size_t hdrlen, unsigned char tag,
			   const void *value, size_t len)
{
	fprintf (context, "got extension\n");
	return 0;
}

static long file_length (FILE *f)
{
	long orig = ftell (f);
	long len;

	if (fseek (f, 0, SEEK_END) != 0	||
	    (len = ftell (f)) < 0	||
	    fseek (f, orig, SEEK_SET) != 0)
		return -1;

	return len;
}

static void *file_read_blob (const char *name, size_t *size)
{
	FILE *f;
	void *blob;
	long len;

	if ((f = fopen (name, "rb")) == NULL)
		return NULL;

	if ((len = file_length (f)) < 0)
		goto no_len;

	if ((blob = malloc (len)) == NULL)
		goto no_mem;

	if (fread (blob, len, 1, f) != 1)
		goto no_read;

	fclose (f);
	*size = len;
	return blob;
no_read:
	free (blob);
no_mem:
no_len:
	fclose (f);
	return NULL;
}

int main (int argc, char *argv[])
{
	const char *name = argv[1];
	void *blob;
	size_t len;

	if (argc != 2)
		errx (1, "usage:\n\tx509-show <X.509-DER-file>\n");

	if ((blob = file_read_blob (name, &len)) == NULL)
		err (1, "cannot get %s blob", name);

	errno = -asn1_ber_decoder (&x509_decoder, stdout, blob, len);
	if (errno != 0)
		err (1, "cannot parse %s", name);

	free (blob);
	return 0;
}
