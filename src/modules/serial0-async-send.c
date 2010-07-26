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

DEFINE_SEND_STR(serial0_async, serial0_async_putc, );
DEFINE_SEND_DATA(serial0_async, serial0_async_putc, );
