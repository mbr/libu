#ifndef SERIAL_H
#define SERIAL_H 

#include <avr/io.h>
#include <inttypes.h>
#include <string.h>

/* block, sending a string string */
void serial0_blocking_send_str(const char *s);
void serial0_blocking_send_data(const char *s, size_t len);

/* setup methods */
void serial0_set_baud_rate(const uint32_t baud_rate);

/* inline methods */
inline static void serial0_putc(char c) {
	UDR0 = c;
}

inline static int serial0_ready_to_send() {
	return UCSR0A & (1 << UDRE0);
}

inline static void serial0_block_until_ready() {
	while(!serial0_ready_to_send());
}

#endif /* SERIAL_H */
