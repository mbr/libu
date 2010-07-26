#ifndef SERIAL_H
#define SERIAL_H 

#include <avr/io.h>
#include <inttypes.h>
#include <string.h>

#include <libu/iomacro.h>

/* setup methods */
void serial0_set_baud_rate(const uint32_t baud_rate);

/* low-level serial i/o */
inline static void serial0_putc(char c) { UDR0 = c; }
inline static char serial0_getc() { return UDR0; }
inline static int serial0_ready_to_send() { return UCSR0A & (1 << UDRE0); }
inline static void serial0_block_until_ready() { while(!serial0_ready_to_send()); }

/* blocking send functions */
static inline DEFINE_SEND(serial0);
DECLARE_SEND_STR(serial0);
DECLARE_SEND_DATA(serial0);

#endif /* SERIAL_H */
