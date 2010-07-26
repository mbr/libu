#ifndef IOMACRO_H
#define IOMACRO_H 

#define DECLARE_SEND_STR(name) void name##_blocking_send_str(const char *s)
#define DEFINE_SEND_STR(name) DECLARE_SEND_STR(name) { while(*s) { name##_block_until_ready(); name##_putc(*(s++)); } }

#define DECLARE_SEND_DATA(name) void name##_blocking_send_data(const char *s, size_t len)
#define DEFINE_SEND_DATA(name) DECLARE_SEND_DATA(name) { while(len) { name##_block_until_ready(); name##_putc(*(s++)); --len; } }

#endif /* IOMACRO_H */
