#include <avr/interrupt.h>
#include <stdlib.h>

#include <libu/bitfiddle.h>
#include <libu/avr/serial.h>

#include <libu/modules/serial0-async-recv.h>
ringbuf_t _serial0_async_recv_rbuf;
static volatile int bytes_until_callback;

static volatile recv_complete_callback recv_complete;

SIGNAL(USART_RX_vect) {
	/* data is ready to read, interrupts are disabled. */
	ringbuf_putc(&_serial0_async_recv_rbuf, serial0_getc());

	if (-1 != bytes_until_callback && serial0_async_recv_available() >= bytes_until_callback) {
		bytes_until_callback = -1;

		sei(); /* reenable interrupts */
		recv_complete();
	}
}

void serial0_async_recv_callback_on(int nbytes, volatile recv_complete_callback callback) {
	recv_complete = callback;
	bytes_until_callback = nbytes;
}

void serial0_async_recv_init(size_t bufsize) {
	char *buffer = malloc(sizeof(char) * bufsize);
	ringbuf_init(&_serial0_async_recv_rbuf, buffer, bufsize, NULL, NULL);

	bytes_until_callback = -1;

	/* enable intterupt */
	SET_BIT(UCSR0B, RXCIE0);
}
