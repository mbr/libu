#include <avr/interrupt.h>
#include <stdlib.h>

#include <libu/bitfiddle.h>
#include <libu/avr/serial.h>

#include <libu/modules/serial0-async-send.h>

ringbuf_t _serial0_async_send_sbuf;

/* FIXME: this may not be support on atmega 128 */
SIGNAL(USART_UDRE_vect) {
	/* interrupts are turned off, and the data register is empty. */
	if (ringbuf_available(&_serial0_async_send_sbuf)) {
		/* data available, send */
		serial0_putc(ringbuf_getc(&_serial0_async_send_sbuf));
	} else {
		/* no data available - clear interrupt handler. will
		 * be set again if more data to send is added. */
		CLEAR_BIT(UCSR0B, UDRIE0);
	}
}

void serial0_async_send_init(size_t bufsize) {
	char *buffer = malloc(sizeof(char) * bufsize);
	ringbuf_init(&_serial0_async_send_sbuf, buffer, bufsize, NULL, NULL);
}

void serial0_async_safe_send(char c) {
	/* wait until buffer is free */
	while(! ringbuf_free_space(&_serial0_async_send_sbuf));

	serial0_async_putc(c);
}

DEFINE_SEND_STR(serial0_async, serial0_async_putc, );
DEFINE_SEND_DATA(serial0_async, serial0_async_putc, );

/* for stdio. */
static int _serial0_async_putc(char c, FILE* stream) {
	serial0_async_safe_send(c);

	return 0;
}

FILE *serial0_async_stdout() {
	FILE *f = malloc(sizeof(FILE));
	fdev_setup_stream(f, _serial0_async_putc, NULL, _FDEV_SETUP_WRITE);
	return f;
}
