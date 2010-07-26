#ifndef IOMACRO_H
#define IOMACRO_H 

#define DECLARE_SEND_STR(name) void name##_send_str(const char *s)
#define DEFINE_SEND_STR(name) DECLARE_SEND_STR(name) { while(*s) { name##_block_until_ready(); name##_putc(*(s++)); } }

#endif /* IOMACRO_H */
