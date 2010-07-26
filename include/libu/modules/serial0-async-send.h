#ifndef SERIAL0_ASYNC_SEND_H
#define SERIAL0_ASYNC_SEND_H 

#include <libu/struct/ringbuf.h>
#include <libu/bitfiddle.h>

extern ringbuf_t _serial0_async_send_sbuf;

inline static void serial0_async_putc(char c) {
	ringbuf_putc(&_serial0_async_send_sbuf, c);

	/* enable Data Register Empty Interrupt */
	SET_BIT(UCSR0B, UDRIE0);
}

void serial0_async_send_init(size_t bufsize);

/* I/O functions */
static inline DEFINE_SEND(serial0_async, serial0_async_putc, );
DECLARE_SEND_STR(serial0_async);
DECLARE_SEND_DATA(serial0_async);

#endif /* SERIAL0-ASYNC-SEND_H */
