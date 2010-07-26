#include <avr/interrupt.h>
#include <stdlib.h>

#include <libu/bitfiddle.h>
#include <libu/avr/serial.h>

#include <libu/modules/serial0-async-recv.h>
ringbuf_t _serial0_async_recv_rbuf;

SIGNAL(USART_RX_vect) {
	/* data is ready to read, interrupts are disabled. */
	ringbuf_putc(&_serial0_async_recv_rbuf, serial0_getc());
}

void serial0_async_recv_init(size_t bufsize) {
	char *buffer = malloc(sizeof(char) * bufsize);
	ringbuf_init(&_serial0_async_recv_rbuf, buffer, bufsize, NULL, NULL);

	/* enable intterupt */
	SET_BIT(UCSR0B, RXCIE0);
}
