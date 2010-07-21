#include <string.h>
#include "struct/ringbuf.h"

void ringbuf_init(ringbuf_t *buf, char *start, size_t len, ringbuf_error_callback on_overflow, ringbuf_error_callback on_underflow) {
	/* assign values */
	buf->start = start;
	buf->len = len;

	buf->writep = buf->start;
	buf->readp = buf->start;

	buf->rcount = 0;
	buf->wcount = 0;

	buf->on_overflow = on_overflow;
	buf->on_underflow = on_underflow;

	/* zero memory area, just to be safe */
	memset(buf->start, 0, buf->len);
}

static inline size_t ringbuf_available(ringbuf_t *buf) {
	/* assumes no buffer overflow has occured */
	if (buf->rcount > buf->wcount) {
		buf->wcount += UINT16_MAX - buf->rcount + 1;
		buf->rcount = 0;
	}

	return buf->len + buf->rcount - buf->wcount;
}

void ringbuf_putc(ringbuf_t *buf, char c) {
	/* check for overflow */
	if (buf->on_overflow && 0 == ringbuf_available(buf)) {
		(buf->on_overflow)(buf);
	}

	/* store */
	*(buf->writep) = c;
	++(buf->wcount);

	/* wraparound */
	if (buf->writep == buf->start+buf->len) buf->writep = buf->start;
}

char ringbuf_getc(ringbuf_t *buf) {
	/* check for underflow */
	if (buf->on_underflow && buf->len == ringbuf_available(buf)) {
		(buf->on_underflow)(buf);
	}

	/* read */
	char c;
	c = *(buf->readp);

	/* wraparound */
	++(buf->rcount);
	if (buf->readp == buf->start+buf->len) buf->readp = buf->start;

	return c;
}
