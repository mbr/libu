#ifndef ARDUINO_H
#define ARDUINO_H 

#include <libu/bitfiddle.h>
#include <avr/io.h>

inline static void led13_toggle() {
	static uint8_t enabled = 0;
	if (enabled) {
		CLEAR_BIT(PORTB, PORTB5);
		enabled = 0;
	} else {
		SET_BIT(PORTB, PORTB5);
		enabled = 1;
	}
}

inline static void led13_activate() {
	SET_BIT(DDRB, PORTB5);
}

#endif /* ARDUINO_H */
