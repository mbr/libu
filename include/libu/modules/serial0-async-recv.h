#ifndef SERIAL0_ASYNC_RECV_H
#define SERIAL0_ASYNC_RECV_H 

#include <libu/struct/ringbuf.h>
#include <libu/bitfiddle.h>

extern ringbuf_t _serial0_async_recv_rbuf;

inline static char serial0_async_recv_getc() {
	return ringbuf_getc(&_serial0_async_recv_rbuf);
}

inline static int serial0_async_recv_available() {
	return ringbuf_available(&_serial0_async_recv_rbuf);
}

void serial0_async_recv_init(size_t bufsize);

/* allow using a callback (interrupt handler calls it) */
typedef void (*recv_complete_callback)();
void serial0_async_recv_callback_on(int nbytes, volatile recv_complete_callback callback);

#endif /* SERIAL0-ASYNC-RECV_H */
