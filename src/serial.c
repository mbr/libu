#include <libu/avr/serial.h>
#include <string.h>
#include <avr/io.h>

#include <libu/avr/serial.h>
#include <inttypes.h>

void blocking_send(const char *s) {
	while(*s) {
		while(!(UCSR0A & (1 << UDRE0)));

		UDR0 = *(s++);	
	}
};

void blocking_send_data(const char *p, size_t len) {
	while(len) {
		while(!(UCSR0A & (1 << UDRE0)));

		UDR0 = *(p++);	
		--len;
	}
}
