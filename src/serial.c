#include <libu/avr/serial.h>

void serial0_set_baud_rate(const uint32_t baud_rate) {
	const uint16_t baud_setting = (uint16_t) (F_CPU/(16*baud_rate) - 1);

	/* set baud rate in registers */
	UBRR0H = (unsigned char) (baud_setting)>>8;
	UBRR0L = (unsigned char) (baud_setting);
};

void serial0_blocking_send(const char *s) {
	while(*s) {
		serial0_block_until_ready();
		serial0_putc(*(s++));
	}
};

void serial0_blocking_send_data(const char *s, size_t len) {
	while(len) {
		serial0_block_until_ready();
		serial0_putc(*(s++));
		--len;
	}
};
