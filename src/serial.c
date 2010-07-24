#include <libu/avr/serial.h>
#include <avr/io.h>

#include <libu/bitfiddle.h>
#include <libu/avr/serial.h>

void blocking_send(const char *s) {
	while(*s) {
		while(!(UCSR0A & (1 << UDRE0)));

		UDR0 = *(s++);	
	}
};
