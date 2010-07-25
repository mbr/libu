#ifndef BITFIDDLE_H
#define BITFIDDLE_H 

#define BIT(n) (1 << (n))

#define SET_BITS(var, mask) (var |= (mask))
#define CLEAR_BITS(var, mask) (var &= (mask))

#define SET_BIT(var, n) (SET_BITS(var, BIT(n)))
#define CLEAR_BIT(var, n) (CLEAR_BITS(var, BIT(n)))

#define BITS_SET(var, mask) (((var) & (mask)) == (mask))
#define BITS_CLEARED(var, mask) ((~(var) & (mask)) == (mask))

#define GET_BIT(var, n) ((var) & BIT(n) >> (n))

#endif /* BITFIDDLE_H */
