#include <libu/avr/serial.h>

DEFINE_SEND_STR(serial0);
DEFINE_SEND_DATA(serial0);

void serial0_set_baud_rate(const uint32_t baud_rate) {
	const uint16_t baud_setting = (uint16_t) (F_CPU/(16*baud_rate) - 1);

	/* set baud rate in registers */
	UBRR0H = (unsigned char) (baud_setting)>>8;
	UBRR0L = (unsigned char) (baud_setting);
};
