#include <libu/avr/serial.h>

DEFINE_SEND_STR(serial0_blocking, serial0_putc, serial0_block_until_ready());
DEFINE_SEND_DATA(serial0_blocking, serial0_putc, serial0_block_until_ready());

void serial0_set_baud_rate(const uint32_t baud_rate) {
	const uint16_t baud_setting = (uint16_t) (F_CPU/(16*baud_rate) - 1);

	/* set baud rate in registers */
	UBRR0H = (unsigned char) (baud_setting)>>8;
	UBRR0L = (unsigned char) (baud_setting);
};
