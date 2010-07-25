#include <string.h>
#include <libu/struct/ringbuf.h>

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

void ringbuf_putc(ringbuf_t *buf, char c) {
	/* check for overflow */
	if (buf->on_overflow && 0 == ringbuf_free_space(buf)) {
		(buf->on_overflow)(buf);
	}

	/* store */
	*(buf->writep) = c;

	/* increment counter and pointer */
	++(buf->wcount);
	++(buf->writep);

	/* wraparound */
	if (buf->writep == buf->start+buf->len) buf->writep = buf->start;
}

char ringbuf_getc(ringbuf_t *buf) {
	/* check for underflow */
	if (buf->on_underflow && buf->len == ringbuf_free_space(buf)) {
		(buf->on_underflow)(buf);
	}

	/* read */
	char c;
	c = *(buf->readp);

	/* increment counter and pointer */
	++(buf->rcount);
	++(buf->readp);

	/* wraparound */
	if (buf->readp == buf->start+buf->len) buf->readp = buf->start;

	return c;
}

void ringbuf_set_overflow_callback(ringbuf_t *buf, ringbuf_error_callback cb) {
	buf->on_overflow = cb;
}

void ringbuf_set_underflow_callback(ringbuf_t *buf, ringbuf_error_callback cb) {
	buf->on_underflow = cb;
}
