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
void ringbuf_set_overflow_callback(ringbuf_t *buf, ringbuf_error_callback cb);
void ringbuf_set_underflow_callback(ringbuf_t *buf, ringbuf_error_callback cb);
static inline size_t ringbuf_free_space(ringbuf_t *buf) {
	/* assumes no buffer overflow has occured */
	if (buf->rcount > buf->wcount) {
		buf->wcount += UINT16_MAX - buf->rcount + 1;
		buf->rcount = 0;
	}

	return buf->len + buf->rcount - buf->wcount;
}
static inline size_t ringbuf_available(ringbuf_t *buf) {
	return buf->len - ringbuf_free_space(buf);
}

#endif
