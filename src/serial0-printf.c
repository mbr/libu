#include <stdlib.h>
#include <libu/avr/serial.h>
#include <libu/avr/serial0-printf.h>

#include <stdio.h>

static int _serial0_blocking_putc(char c, FILE* stream) {
	serial0_blocking_send(c);

	return 0;
}

FILE *serial0_blocking_stdout() {
	FILE *f = malloc(sizeof(FILE));
	fdev_setup_stream(f, _serial0_blocking_putc, NULL, _FDEV_SETUP_WRITE);
	return f;
}
