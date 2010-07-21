#ifndef ULIB_STRUCT_RINGBUF_H
#define ULIB_STRUCT_RINGBUF_H

#include <stdint.h>

struct _ringbuf_t;
typedef void (*ringbuf_error_callback)(struct _ringbuf_t *);

typedef struct _ringbuf_t {
	char *start;
	size_t len;

	char *writep;
	char *readp;

	uint16_t rcount;
	uint16_t wcount;
	
	/* on overflow */
	ringbuf_error_callback on_overflow;
	ringbuf_error_callback on_underflow;

} ringbuf_t;


void ringbuf_init(ringbuf_t *buf, char *start, size_t len, ringbuf_error_callback on_overflow, ringbuf_error_callback on_underflow);
void ringbuf_putc(ringbuf_t *buf, char c);
char ringbuf_getc(ringbuf_t *buf);

#endif
